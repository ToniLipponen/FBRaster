#ifndef IMAGE_H_
#define IMAGE_H_
#include "Vector.h"

typedef struct Image
{
    int width, height, bpp;
    unsigned char* data;
}Image;

void LoadImage(const char* filename, Image* image);
void FreeImage(Image* image);

Vec4 SampleImage(Image* image, Vec2 uv);

#endif