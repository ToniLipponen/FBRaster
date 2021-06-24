#ifndef VERTEX_H_
#define VERTEX_H_
#include "Vector.h"

struct Vertex
{
    Vec4 pos;
    Vec4 col;
    Vec2 uv;
};
typedef struct Vertex Vertex;


inline static Vertex VertexInterp(Vertex* a, Vertex* b, const float v)
{
    const float d = v / DistancePtr(&a->pos, &b->pos);
    return (Vertex)
    {
        (a->pos * d) + (b->pos * (1.f - d)),
        (a->col * d) + (b->col * (1.f - d)),
        (a->uv  * d) + (b->uv  * (1.f - d))
    };
}

inline static float VertexInterpPtr(Vertex* ptr, Vertex* a, Vertex* b, const float v)
{
    const float d = v / DistancePtr(&a->pos, &b->pos);
    ptr->pos = (a->pos * d) + (b->pos * (1.f - d));
    ptr->col = (a->col * d) + (b->col * (1.f - d));
    ptr->uv  = (a->uv  * d) + (b->uv  * (1.f - d));
    return d;
}
#endif
