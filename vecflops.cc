// x86 AVX FLOPs counter
// Based on:
//   http://stackoverflow.com/questions/8389648/how-to-achieve-4-flops-per-cycle

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "openmp.h"
using namespace std;

// define 1 to get single precision and 2x flops boost
#define SINGLE_PRECISION 0

#if SINGLE_PRECISION

#ifdef __AVX__
#define SIMD 8
#else
#define SIMD 4
#endif
typedef float   float_t;
typedef int32_t  int_t;
typedef uint32_t uint_t;
#define OVERFLOW_MASK 0x80fffffu

#else

#ifdef __AVX__
#define SIMD 4
#else
#define SIMD 2
#endif
typedef double   float_t;
typedef int64_t  int_t;
typedef uint64_t uint_t;
#define OVERFLOW_MASK 0x800fffffffffffffull

#endif

#define dupn(num)   dup ## num
#define dup(x, num) dupn(num)(x)
#define dup1(x)  x
#define dup2(x)  x, x
#define dup4(x)  dup2(x),  dup2(x)
#define dup8(x)  dup4(x),  dup4(x)
#define dup16(x) dup8(x),  dup8(x)
#define dup32(x) dup16(x), dup16(x)
#define dup64(x) dup32(x), dup32(x)

typedef float_t fvec_t __attribute__((vector_size((SIMD)*sizeof(float_t))));
typedef int_t   ivec_t __attribute__((vector_size((SIMD)*sizeof(int_t))));
typedef uint_t  uvec_t __attribute__((vector_size((SIMD)*sizeof(uint_t))));
#define fvec_t(x) (fvec_t){ dup((float_t)x, SIMD) }
#define uvec_t(x) (uvec_t){ dup( (uint_t)x, SIMD) }

float_t test_dp_mac_gen(float_t x,float_t y,size_t iterations){
	register fvec_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC,rD,rE,rF;

	// Generate starting data.
	r0 = fvec_t( x);
	r1 = fvec_t( y);
	r2 = fvec_t( y);
	r3 = fvec_t( 1.0);
	r4 = fvec_t(-1.0);
	r5 = fvec_t( 1.0);
	r6 = fvec_t(-1.0);
	r7 = fvec_t( 1.0);
	r8 = fvec_t(-1.0);
	r9 = fvec_t( 1.0);
	rA = fvec_t(-1.0);
	rB = fvec_t( 1.0);
	rC = fvec_t(-x);
	rD = fvec_t( x);
	rE = fvec_t(-y);
	rF = fvec_t( y);

	uvec_t MASK = uvec_t(OVERFLOW_MASK);
	uvec_t vONE = (uvec_t)fvec_t(1.0);

	size_t c = 0;
	while (c < iterations){
		size_t i = 0;
		while (i < 1000){
			//  Here's the meat - the part that really matters.
			r0 = r0 * rC;
			r1 = r1 + rD;
			r2 = r2 * rE;
			r3 = r3 - rF;
			r4 = r4 * rC;
			r5 = r5 + rD;
			r6 = r6 * rE;
			r7 = r7 - rF;
			r8 = r8 * rC;
			r9 = r9 + rD;
			rA = rA * rE;
			rB = rB - rF;

			r0 = r0 + rF;
			r1 = r1 * rE;
			r2 = r2 - rD;
			r3 = r3 * rC;
			r4 = r4 + rF;
			r5 = r5 * rE;
			r6 = r6 - rD;
			r7 = r7 * rC;
			r8 = r8 + rF;
			r9 = r9 * rE;
			rA = rA - rD;
			rB = rB * rC;

			r0 = r0 * rC;
			r1 = r1 + rD;
			r2 = r2 * rE;
			r3 = r3 - rF;
			r4 = r4 * rC;
			r5 = r5 + rD;
			r6 = r6 * rE;
			r7 = r7 - rF;
			r8 = r8 * rC;
			r9 = r9 + rD;
			rA = rA * rE;
			rB = rB - rF;

			r0 = r0 + rF;
			r1 = r1 * rE;
			r2 = r2 - rD;
			r3 = r3 * rC;
			r4 = r4 + rF;
			r5 = r5 * rE;
			r6 = r6 - rD;
			r7 = r7 * rC;
			r8 = r8 + rF;
			r9 = r9 * rE;
			rA = rA - rD;
			rB = rB * rC;

			i++;
		}

		//  Need to renormalize to prevent denormal/overflow.
		r0 = (fvec_t)((uvec_t)r0 & MASK);
		r1 = (fvec_t)((uvec_t)r1 & MASK);
		r2 = (fvec_t)((uvec_t)r2 & MASK);
		r3 = (fvec_t)((uvec_t)r3 & MASK);
		r4 = (fvec_t)((uvec_t)r4 & MASK);
		r5 = (fvec_t)((uvec_t)r5 & MASK);
		r6 = (fvec_t)((uvec_t)r6 & MASK);
		r7 = (fvec_t)((uvec_t)r7 & MASK);
		r8 = (fvec_t)((uvec_t)r8 & MASK);
		r9 = (fvec_t)((uvec_t)r9 & MASK);
		rA = (fvec_t)((uvec_t)rA & MASK);
		rB = (fvec_t)((uvec_t)rB & MASK);
		r0 = (fvec_t)((uvec_t)r0 | vONE);
		r1 = (fvec_t)((uvec_t)r1 | vONE);
		r2 = (fvec_t)((uvec_t)r2 | vONE);
		r3 = (fvec_t)((uvec_t)r3 | vONE);
		r4 = (fvec_t)((uvec_t)r4 | vONE);
		r5 = (fvec_t)((uvec_t)r5 | vONE);
		r6 = (fvec_t)((uvec_t)r6 | vONE);
		r7 = (fvec_t)((uvec_t)r7 | vONE);
		r8 = (fvec_t)((uvec_t)r8 | vONE);
		r9 = (fvec_t)((uvec_t)r9 | vONE);
		rA = (fvec_t)((uvec_t)rA | vONE);
		rB = (fvec_t)((uvec_t)rB | vONE);

		c++;
	}

	r0 = r0 + r1;
	r2 = r2 + r3;
	r4 = r4 + r5;
	r6 = r6 + r7;
	r8 = r8 + r9;
	rA = rA + rB;

	r0 = r0 + r2;
	r4 = r4 + r6;
	r8 = r8 + rA;

	r0 = r0 + r4;
	r0 = r0 + r8;

	float_t out = (float_t)0;
	for(size_t s = 0; s < SIMD; s++) out += ((float_t *)&r0)[s];
	return out;
}

void test_dp_mac_gen(int tds,size_t iterations){

	float_t *sum = (float_t*)malloc(tds * sizeof(float_t));
	float_t start = get_wtime();

#pragma omp parallel num_threads(tds)
	{
		float_t ret = test_dp_mac_gen((float_t)1.1,(float_t)2.1,iterations);
		sum[omp_get_thread_num()] = ret;
	}

	float_t secs = get_wtime() - start;
	uint64_t ops = 48 * 1000 * iterations * tds * SIMD;
	cout << "Seconds = " << secs << endl;
	cout << "FP Ops  = " << ops << endl;
	cout << "GFLOPs  = " << ops / 1e9 / secs << endl;

	float_t out = 0;
	int c = 0;
	while (c < tds){
		out += sum[c++];
	}

	cout << "sum = " << out << endl;
	cout << endl;

	free(sum);
}

int main(int argc, char const *argv[])
{
	test_dp_mac_gen(omp_get_max_threads(), argc >= 2 ? atol(argv[1]) : 1000000);
	return 0;
}
