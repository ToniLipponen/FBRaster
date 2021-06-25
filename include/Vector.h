#ifndef VECTOR_H_
#define VECTOR_H_

#include <sys/cdefs.h>
#include <xmmintrin.h>
#include <x86intrin.h>
#include <math.h>

typedef float Vec4 __attribute__ ((vector_size(16), __aligned__(16)));
typedef float Vec2 __attribute__ ((vector_size(8),  __aligned__(8)));

typedef Vec4 Color;

static inline __m128 _mm_abs_ps_2(__m128 x) {
  __m128 signMask = _mm_set1_ps(-0.0F);
  return _mm_andnot_ps(signMask, x);
}

static inline float Vec4_Dot(Vec4 a, Vec4 b)
{
    Vec4 nv = a * b;
    nv = _mm_hadd_ps(nv, nv);
    nv = _mm_hadd_ps(nv, nv);
    return nv[0];
}

static inline Vec4 Vec4_Cross(Vec4 a, Vec4 b)
{
    return ((Vec4){a[1], a[2], a[0],0} * (Vec4){b[2], b[0], b[1], 0}) -
           ((Vec4){a[2], a[0], a[1],0} * (Vec4){b[1], b[2], b[0], 0});
            
}

static inline float Distance(Vec4 a, Vec4 b)
{
    Vec4 nv = a - b;
    nv = nv * nv;
    nv = _mm_hadd_ps(nv, nv);
    nv = _mm_hadd_ps(nv, nv);
    return sqrt(nv[0]);
}

static inline float DistancePtr(Vec4* a, Vec4* b)
{
    Vec4 nv = *a - *b;
    nv = nv * nv;
    nv = _mm_hadd_ps(nv, nv);
    nv = _mm_hadd_ps(nv, nv);
    return sqrt(nv[0]);
}

static inline float Vec4_Length(Vec4 v)
{
    return sqrt(Vec4_Dot(v, v));
}

static inline Vec4 Normalize(Vec4 vec)
{
	return vec / Vec4_Length(vec);
}

inline static Vec4 triangle_normal(Vec4 a, Vec4 b, Vec4 c)
{
	a = Normalize(a);
	b = Normalize(b);
	c = Normalize(c);
	return Normalize(Vec4_Cross((b - a), (c - a)));
}

#endif
