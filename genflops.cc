// x86 AVX FLOPs counter
// Based on:
//   http://stackoverflow.com/questions/8389648/how-to-achieve-4-flops-per-cycle

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "openmp.h"
using namespace std;

#define SIMD 8
typedef float float_t;
typedef float_t simd_t[SIMD];

#define LOOP for(size_t s = 0; s < SIMD; s++)

float_t test_dp_mac_gen(float_t x,float_t y,size_t iterations){
	register simd_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC,rD,rE,rF;

    // Generate starting data.
	LOOP r0[s] = x;
	LOOP r1[s] = y;
	LOOP r2[s] = y;
	LOOP r3[s] = (float_t) 1.0;
	LOOP r4[s] = (float_t)-1.0;
	LOOP r5[s] = (float_t) 1.0;
	LOOP r6[s] = (float_t)-1.0;
	LOOP r7[s] = (float_t) 1.0;
	LOOP r8[s] = (float_t)-1.0;
	LOOP r9[s] = (float_t) 1.0;
	LOOP rA[s] = (float_t)-1.0;
	LOOP rB[s] = (float_t) 1.0;
	LOOP rC[s] = (float_t)-1.0;
	LOOP rD[s] = (float_t) 1.0;
	LOOP rE[s] = (float_t)-1.0;
	LOOP rF[s] = (float_t) 1.0;

	for (size_t c = 0; c < iterations * 1000; c++) {
		LOOP r0[s] = r0[s] * rC[s];
		LOOP r1[s] = r1[s] + rD[s];
		LOOP r2[s] = r2[s] * rE[s];
		LOOP r3[s] = r3[s] - rF[s];
		LOOP r4[s] = r4[s] * rC[s];
		LOOP r5[s] = r5[s] + rD[s];
		LOOP r6[s] = r6[s] * rE[s];
		LOOP r7[s] = r7[s] - rF[s];
		LOOP r8[s] = r8[s] * rC[s];
		LOOP r9[s] = r9[s] + rD[s];
		LOOP rA[s] = rA[s] * rE[s];
		LOOP rB[s] = rB[s] - rF[s];

		LOOP r0[s] = r0[s] + rF[s];
		LOOP r1[s] = r1[s] * rE[s];
		LOOP r2[s] = r2[s] - rD[s];
		LOOP r3[s] = r3[s] * rC[s];
		LOOP r4[s] = r4[s] + rF[s];
		LOOP r5[s] = r5[s] * rE[s];
		LOOP r6[s] = r6[s] - rD[s];
		LOOP r7[s] = r7[s] * rC[s];
		LOOP r8[s] = r8[s] + rF[s];
		LOOP r9[s] = r9[s] * rE[s];
		LOOP rA[s] = rA[s] - rD[s];
		LOOP rB[s] = rB[s] * rC[s];

		LOOP r0[s] = r0[s] * rC[s];
		LOOP r1[s] = r1[s] + rD[s];
		LOOP r2[s] = r2[s] * rE[s];
		LOOP r3[s] = r3[s] - rF[s];
		LOOP r4[s] = r4[s] * rC[s];
		LOOP r5[s] = r5[s] + rD[s];
		LOOP r6[s] = r6[s] * rE[s];
		LOOP r7[s] = r7[s] - rF[s];
		LOOP r8[s] = r8[s] * rC[s];
		LOOP r9[s] = r9[s] + rD[s];
		LOOP rA[s] = rA[s] * rE[s];
		LOOP rB[s] = rB[s] - rF[s];

		LOOP r0[s] = r0[s] + rF[s];
		LOOP r1[s] = r1[s] * rE[s];
		LOOP r2[s] = r2[s] - rD[s];
		LOOP r3[s] = r3[s] * rC[s];
		LOOP r4[s] = r4[s] + rF[s];
		LOOP r5[s] = r5[s] * rE[s];
		LOOP r6[s] = r6[s] - rD[s];
		LOOP r7[s] = r7[s] * rC[s];
		LOOP r8[s] = r8[s] + rF[s];
		LOOP r9[s] = r9[s] * rE[s];
		LOOP rA[s] = rA[s] - rD[s];
		LOOP rB[s] = rB[s] * rC[s];
	}

	LOOP r0[s] = r0[s] + r1[s];
	LOOP r2[s] = r2[s] + r3[s];
	LOOP r4[s] = r4[s] + r5[s];
	LOOP r6[s] = r6[s] + r7[s];
	LOOP r8[s] = r8[s] + r9[s];
	LOOP rA[s] = rA[s] + rB[s];

	LOOP r0[s] = r0[s] + r2[s];
	LOOP r4[s] = r4[s] + r6[s];
	LOOP r8[s] = r8[s] + rA[s];

	LOOP r0[s] = r0[s] + r4[s];
	LOOP r0[s] = r0[s] + r8[s];

	float_t out = (float_t)0;
	LOOP out += r0[s];
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
