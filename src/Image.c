#include "../include/Image.h"
#include "../include/Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void LoadImage(const char* filename, Image* image)
{
    stbi_set_flip_vertically_on_load(1);
    image->data = stbi_load(filename, &image->width, &image->height, &image->bpp, 3);
}

void FreeImage(Image* image)
{
    if(image->data)
        free(image->data);
}

Vec4 SampleImage(Image* image, Vec2 uv)
{
    Vec4 color = {0};
    const int x = uv[0] * image->width;
    const int y = uv[1] * image->height;
    if(x < image->width && x >= 0 && y < image->height && y >= 0)
    {
        color[0] = image->data[clampi((y * image->width * image->bpp) + x*image->bpp,   0, image->width * image->height * image->bpp)];
        color[1] = image->data[clampi((y * image->width * image->bpp) + x*image->bpp+1, 0, image->width * image->height * image->bpp)];
        color[2] = image->data[clampi((y * image->width * image->bpp) + x*image->bpp+2, 0, image->width * image->height * image->bpp)];
        color[3] = 255.f;
    }

    return color;
}