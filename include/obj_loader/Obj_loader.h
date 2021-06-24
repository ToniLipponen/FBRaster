#ifndef VERY_SIMPLE_OBJ_LOADER_H_
#define VERY_SIMPLE_OBJ_LOADER_H_
#include "../Vertex.h"

typedef struct 
{
    Vertex* vertices;
}Wavefront_Obj;

Vertex* LoadObj(const char* filename)
{

}

#endif