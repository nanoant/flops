// x86 SSE FLOPs counter
// Based on:
//   http://stackoverflow.com/questions/8389648/how-to-achieve-4-flops-per-cycle

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "openmp.h"
using namespace std;

#include <immintrin.h>
double test_dp_mac_SSE(double x,double y,size_t iterations){
	register __m128d r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC,rD,rE,rF;

	// Generate starting data.
	r0 = _mm_set1_pd( x);
	r1 = _mm_set1_pd( y);
	r2 = _mm_set1_pd( y);
	r3 = _mm_set1_pd( 1.0);
	r4 = _mm_set1_pd(-1.0);
	r5 = _mm_set1_pd( 1.0);
	r6 = _mm_set1_pd(-1.0);
	r7 = _mm_set1_pd( 1.0);
	r8 = _mm_set1_pd(-1.0);
	r9 = _mm_set1_pd( 1.0);
	rA = _mm_set1_pd(-1.0);
	rB = _mm_set1_pd( 1.0);
	rC = _mm_set1_pd( x);
	rD = _mm_set1_pd(-x);
	rE = _mm_set1_pd(-y);
	rF = _mm_set1_pd( y);

	uint64_t iMASK = 0x800fffffffffffffull;
	__m128d MASK = _mm_set1_pd(*(double*)&iMASK);
	__m128d vONE = _mm_set1_pd(1.0);

	size_t c = 0;
	while (c < iterations){
		size_t i = 0;
		while (i < 1000){
			//  Here's the meat - the part that really matters.

			r0 = _mm_mul_pd(r0,rC);
			r1 = _mm_add_pd(r1,rD);
			r2 = _mm_mul_pd(r2,rE);
			r3 = _mm_sub_pd(r3,rF);
			r4 = _mm_mul_pd(r4,rC);
			r5 = _mm_add_pd(r5,rD);
			r6 = _mm_mul_pd(r6,rE);
			r7 = _mm_sub_pd(r7,rF);
			r8 = _mm_mul_pd(r8,rC);
			r9 = _mm_add_pd(r9,rD);
			rA = _mm_mul_pd(rA,rE);
			rB = _mm_sub_pd(rB,rF);

			r0 = _mm_add_pd(r0,rF);
			r1 = _mm_mul_pd(r1,rE);
			r2 = _mm_sub_pd(r2,rD);
			r3 = _mm_mul_pd(r3,rC);
			r4 = _mm_add_pd(r4,rF);
			r5 = _mm_mul_pd(r5,rE);
			r6 = _mm_sub_pd(r6,rD);
			r7 = _mm_mul_pd(r7,rC);
			r8 = _mm_add_pd(r8,rF);
			r9 = _mm_mul_pd(r9,rE);
			rA = _mm_sub_pd(rA,rD);
			rB = _mm_mul_pd(rB,rC);

			r0 = _mm_mul_pd(r0,rC);
			r1 = _mm_add_pd(r1,rD);
			r2 = _mm_mul_pd(r2,rE);
			r3 = _mm_sub_pd(r3,rF);
			r4 = _mm_mul_pd(r4,rC);
			r5 = _mm_add_pd(r5,rD);
			r6 = _mm_mul_pd(r6,rE);
			r7 = _mm_sub_pd(r7,rF);
			r8 = _mm_mul_pd(r8,rC);
			r9 = _mm_add_pd(r9,rD);
			rA = _mm_mul_pd(rA,rE);
			rB = _mm_sub_pd(rB,rF);

			r0 = _mm_add_pd(r0,rF);
			r1 = _mm_mul_pd(r1,rE);
			r2 = _mm_sub_pd(r2,rD);
			r3 = _mm_mul_pd(r3,rC);
			r4 = _mm_add_pd(r4,rF);
			r5 = _mm_mul_pd(r5,rE);
			r6 = _mm_sub_pd(r6,rD);
			r7 = _mm_mul_pd(r7,rC);
			r8 = _mm_add_pd(r8,rF);
			r9 = _mm_mul_pd(r9,rE);
			rA = _mm_sub_pd(rA,rD);
			rB = _mm_mul_pd(rB,rC);

			i++;
		}

        //  Need to renormalize to prevent denormal/overflow.
		r0 = _mm_and_pd(r0,MASK);
		r1 = _mm_and_pd(r1,MASK);
		r2 = _mm_and_pd(r2,MASK);
		r3 = _mm_and_pd(r3,MASK);
		r4 = _mm_and_pd(r4,MASK);
		r5 = _mm_and_pd(r5,MASK);
		r6 = _mm_and_pd(r6,MASK);
		r7 = _mm_and_pd(r7,MASK);
		r8 = _mm_and_pd(r8,MASK);
		r9 = _mm_and_pd(r9,MASK);
		rA = _mm_and_pd(rA,MASK);
		rB = _mm_and_pd(rB,MASK);
		r0 = _mm_or_pd(r0,vONE);
		r1 = _mm_or_pd(r1,vONE);
		r2 = _mm_or_pd(r2,vONE);
		r3 = _mm_or_pd(r3,vONE);
		r4 = _mm_or_pd(r4,vONE);
		r5 = _mm_or_pd(r5,vONE);
		r6 = _mm_or_pd(r6,vONE);
		r7 = _mm_or_pd(r7,vONE);
		r8 = _mm_or_pd(r8,vONE);
		r9 = _mm_or_pd(r9,vONE);
		rA = _mm_or_pd(rA,vONE);
		rB = _mm_or_pd(rB,vONE);

		c++;
	}

	r0 = _mm_add_pd(r0,r1);
	r2 = _mm_add_pd(r2,r3);
	r4 = _mm_add_pd(r4,r5);
	r6 = _mm_add_pd(r6,r7);
	r8 = _mm_add_pd(r8,r9);
	rA = _mm_add_pd(rA,rB);

	r0 = _mm_add_pd(r0,r2);
	r4 = _mm_add_pd(r4,r6);
	r8 = _mm_add_pd(r8,rA);

	r0 = _mm_add_pd(r0,r4);
	r0 = _mm_add_pd(r0,r8);

    //  Prevent Dead Code Elimination
	double out = 0;
	out += ((double*)&r0)[0];
	out += ((double*)&r0)[1];

	return out;
}

void test_dp_mac_SSE(int tds,size_t iterations){

	double *sum = (double*)malloc(tds * sizeof(double));
	double start = get_wtime();

#pragma omp parallel num_threads(tds)
	{
		double ret = test_dp_mac_SSE(1.1,2.1,iterations);
		sum[omp_get_thread_num()] = ret;
	}

	double secs = get_wtime() - start;
	uint64_t ops = 48 * 1000 * iterations * tds * 2;
	cout << "Seconds = " << secs << endl;
	cout << "FP Ops  = " << ops << endl;
	cout << "GFLOPs  = " << ops / 1e9 / secs << endl;

	double out = 0;
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
	test_dp_mac_SSE(omp_get_max_threads(), argc >= 2 ? atol(argv[1]) : 1000000);
	return 0;
}
