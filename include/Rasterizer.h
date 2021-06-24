#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include "Vector.h"
#include "Vertex.h"
#include "Image.h"

#define POINTS              0x1
#define LINES               0x2
#define LINES_STRIP         0x3
#define LINES_LOOP          0x4
#define TRIANGLES           0x5
#define TRIANGLES_STRIP     0x6
#define TRIANGLES_LOOP      0x7

#define DEPTH_BUFFER_BIT 0x1
#define COLOR_BUFFER_BIT 0x2

void tlInitialize();
void tlClear(unsigned int mask);
void tlClearColor(int color);
void tlSetShaders(
    void (*Vertex) (Vertex* vertex), 
    void (*fragment)(struct Vertex* vertex, Color* output));
void tlDrawBuffer(Vertex* buffer, unsigned int elements);
void tlDrawBufferIndexed(unsigned int mode, Vertex* buffer, unsigned int* indexbuffer, unsigned int elements);
void tlSwapBuffers();
void tlDestroy();
#endif
