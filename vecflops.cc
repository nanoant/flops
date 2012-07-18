// x86 AVX FLOPs counter
// Based on:
//   http://stackoverflow.com/questions/8389648/how-to-achieve-4-flops-per-cycle

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "openmp.h"
using namespace std;

#define SIMD 4
typedef double float_t;

#define dupn(num)   dup ## num
#define dup(x, num) dupn(num)(x)
#define dup1(x)  x
#define dup2(x)  x, x
#define dup4(x)  dup2(x),  dup2(x)
#define dup8(x)  dup4(x),  dup4(x)
#define dup16(x) dup8(x),  dup8(x)
#define dup32(x) dup16(x), dup16(x)
#define dup64(x) dup32(x), dup32(x)

#if __clang__

typedef float_t   simd_t __attribute__((ext_vector_type(SIMD)));
typedef uint64_t isimd_t __attribute__((ext_vector_type(SIMD)));
#define simd_t(x)   (simd_t)((float_t)x)
#define isimd_t(x) (isimd_t)((float_t)x)

#else

#define VSIMD SIMD*sizeof(float_t)
typedef float_t   simd_t __attribute__((vector_size(VSIMD)));
typedef uint64_t isimd_t __attribute__((vector_size(VSIMD)));
#define simd_t(x)   (simd_t){dup((float_t)x, SIMD)}
#define isimd_t(x) (isimd_t){dup((uint64_t)x, SIMD)}

#endif

float_t test_dp_mac_gen(float_t x,float_t y,size_t iterations){
	register simd_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC,rD,rE,rF;

	// Generate starting data.
	r0 = simd_t( x);
	r1 = simd_t( y);
	r2 = simd_t( y);
	r3 = simd_t( 1.0);
	r4 = simd_t(-1.0);
	r5 = simd_t( 1.0);
	r6 = simd_t(-1.0);
	r7 = simd_t( 1.0);
	r8 = simd_t(-1.0);
	r9 = simd_t( 1.0);
	rA = simd_t(-1.0);
	rB = simd_t( 1.0);
	rC = simd_t(-x);
	rD = simd_t( x);
	rE = simd_t(-y);
	rF = simd_t( y);

	isimd_t MASK = isimd_t(0x800fffffffffffffull);
	isimd_t vONE = (isimd_t)simd_t(1.0);

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
		r0 = (simd_t)((isimd_t)r0 & MASK);
		r1 = (simd_t)((isimd_t)r1 & MASK);
		r2 = (simd_t)((isimd_t)r2 & MASK);
		r3 = (simd_t)((isimd_t)r3 & MASK);
		r4 = (simd_t)((isimd_t)r4 & MASK);
		r5 = (simd_t)((isimd_t)r5 & MASK);
		r6 = (simd_t)((isimd_t)r6 & MASK);
		r7 = (simd_t)((isimd_t)r7 & MASK);
		r8 = (simd_t)((isimd_t)r8 & MASK);
		r9 = (simd_t)((isimd_t)r9 & MASK);
		rA = (simd_t)((isimd_t)rA & MASK);
		rB = (simd_t)((isimd_t)rB & MASK);
		r0 = (simd_t)((isimd_t)r0 | vONE);
		r1 = (simd_t)((isimd_t)r1 | vONE);
		r2 = (simd_t)((isimd_t)r2 | vONE);
		r3 = (simd_t)((isimd_t)r3 | vONE);
		r4 = (simd_t)((isimd_t)r4 | vONE);
		r5 = (simd_t)((isimd_t)r5 | vONE);
		r6 = (simd_t)((isimd_t)r6 | vONE);
		r7 = (simd_t)((isimd_t)r7 | vONE);
		r8 = (simd_t)((isimd_t)r8 | vONE);
		r9 = (simd_t)((isimd_t)r9 | vONE);
		rA = (simd_t)((isimd_t)rA | vONE);
		rB = (simd_t)((isimd_t)rB | vONE);

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
	float_t start = omp_get_wtime();

#pragma omp parallel num_threads(tds)
	{
		float_t ret = test_dp_mac_gen((float_t)1.1,(float_t)2.1,iterations);
		sum[omp_get_thread_num()] = ret;
	}

	float_t secs = omp_get_wtime() - start;
	uint64_t ops = 48 * 1000 * iterations * tds * 4;
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
