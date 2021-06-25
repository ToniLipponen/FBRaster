#pragma once
#include "Vector.h"
#include "Utility.h"

typedef struct { Vec4 row[4]; } Mat4x4;

static inline Vec4 MulVec4Mat4(Vec4 v, Mat4x4 m)
{
    Vec4 nv;
    nv[0] = Vec4_Dot(v, m.row[0]);
    nv[1] = Vec4_Dot(v, m.row[1]);
    nv[2] = Vec4_Dot(v, m.row[2]);
    nv[3] = Vec4_Dot(v, m.row[3]);
    return nv;
}

static inline Mat4x4 MulMat4x4(Mat4x4 a, Mat4x4 b)
{
    Mat4x4 m = {0};
    m.row[0] = MulVec4Mat4(a.row[0], b);
    m.row[1] = MulVec4Mat4(a.row[1], b);
    m.row[2] = MulVec4Mat4(a.row[2], b);
    m.row[3] = MulVec4Mat4(a.row[3], b);
    return m;
}
static inline Mat4x4 ScaleMat4X4(float x, float y, float z)
{
    Mat4x4 m;
    m.row[0] = (Vec4){x, 0, 0, 0};
    m.row[1] = (Vec4){0, y, 0, 0};
    m.row[2] = (Vec4){0, 0, z, 0};
    m.row[3] = (Vec4){0, 0, 0, 1};
    return m;
}
static inline Mat4x4 TranslateMat4X4(Vec4 v)
{
    Mat4x4 m;
    m.row[0] = (Vec4){1, 0, 0, v[0]};
    m.row[1] = (Vec4){0, 1, 0, v[1]};
    m.row[2] = (Vec4){0, 0, 1, v[2]};
    m.row[3] = (Vec4){0, 0, 0, 1};
    return m;
}

static inline Mat4x4 RotateMat4X4(float x, float y, float z)
{
    Mat4x4 m1, m2, m3;
    
    m1.row[0] = (Vec4){cos(z*0.01745329f), -sin(z*0.01745329f),  0, 0};
    m1.row[1] = (Vec4){sin(z*0.01745329f), cos(z*0.01745329f),   0, 0};
    m1.row[2] = (Vec4){0, 0,             1, 0};
    m1.row[3] = (Vec4){0, 0,             0, 1};

    m2.row[0] = (Vec4){1, 0, 0, 0};
    m2.row[1] = (Vec4){0, cos(x*0.01745329f), -sin(x*0.01745329f), 0};
    m2.row[2] = (Vec4){0, sin(x*0.01745329f), cos(x*0.01745329f), 0};
    m2.row[3] = (Vec4){0, 0, 0, 1};

    m3.row[0] = (Vec4){cos(y*0.01745329f), 0, -sin(y*0.01745329f), 0};
    m3.row[1] = (Vec4){0, 1, 0, 0};
    m3.row[2] = (Vec4){sin(y*0.01745329f), 0, cos(y*0.01745329f), 0};
    m3.row[3] = (Vec4){0, 0, 0, 1};
    m1 = MulMat4x4(m1, m2);
    m1 = MulMat4x4(m1, m3);
    return m1;
}
static inline Mat4x4 Projection(float left, float right, float bottom, float top, float near, float far) {
    Mat4x4 m;
    
    m.row[0] = (Vec4){(2.f*near)/(right-left), 0, (right+left)/(right-left), 0};
    m.row[1] = (Vec4){0, (2.f*near)/(top-bottom), (top+bottom)/(top-bottom), 0};
    m.row[2] = (Vec4){0,0, -(far+near)  / (far-near), -(2*far*near)/ (far-near)};
    m.row[3] = (Vec4){0,0,-1,0};
    return m;
}

static inline Mat4x4 Orthographic(float left, float right, float bottom, float top, float near, float far)
{
    Mat4x4 m;
    m.row[0] = (Vec4){2.f/(right-left), 0, 0, -((right+left)/(right-left))};
    m.row[1] = (Vec4){0, 2.f/(top-bottom), 0, -((top+bottom)/(top-bottom))};
    m.row[2] = (Vec4){0, 0, -2.f/(far-near), -((far+near)/(far-near))};
    m.row[3] = (Vec4){0, 0, 0, 1};
    return m;
}

static inline Mat4x4 Identity()
{
    Mat4x4 m;
    m.row[0] = (Vec4){1, 0, 0, 0};
    m.row[1] = (Vec4){0, 1, 0, 0};
    m.row[2] = (Vec4){0, 0, 1, 0};
    m.row[3] = (Vec4){0, 0, 0, 1};
    return m;
}
