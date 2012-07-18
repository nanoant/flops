// Compile with C not C++, as G++ misses some vector operators in C++ mode
// cc -mavx -O3 -c -o simd_capabilities.o simd_capabilities.c

// Peek into machine code
// objdump -d simd_capabilities.o

// NOTES:
// http://lists.cs.uiuc.edu/pipermail/llvmdev/2012-July/051470.html

#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

#define SIMD 8

#define simd_int_t(s)   int ## (s*8) ## _t
#define simd_uint_t(s) uint ## (s*8) ## _t

typedef float    float_t;
typedef int32_t  int_t;
typedef uint32_t uint_t;

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
#define float_t(x, c) (((float_t *)&x)[c])
#define int_t(x, c)     (((int_t *)&x)[c])
#define uint_t(x, c)   (((uint_t *)&x)[c])

fvec_t faddps(fvec_t a, fvec_t b) { return a + b; }
fvec_t fmulps(fvec_t a, fvec_t b) { return a * b; }
fvec_t fmadps(fvec_t a, fvec_t b, fvec_t c) { return c + a * b; }
#ifndef __ICC
uvec_t fcmpps(fvec_t a, fvec_t b) { return (a < b); }
uvec_t ucmpps(fvec_t a, fvec_t b) { return (a < b); }
uvec_t uxorps(uvec_t a, uvec_t b) { return a ^ b; }
uvec_t ushlps(uvec_t a, uint_t s) {
#if SIMD == 8
#else
	return (a << uvec_t(s));
#endif
}
fvec_t fminps(fvec_t a, fvec_t b) {
	uvec_t c = (a < b);
	return (fvec_t)(((uvec_t)a & c) | ((uvec_t)b & ~c));
}
#endif

#if 0
int main (int argc, char const *argv[])
{
	ivec_t a = {1, 2, 3, 4};
	ivec_t b = {3, 2, 1, 0};
	ivec_t m = min(a, b);
	printf("%d %d %d %d\n", int_t(m, 0), int_t(m, 1), int_t(m, 2), int_t(m, 3));
	return 0;
}
#endif
