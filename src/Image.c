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
	const Vec2 p = uv * (Vec2){image->width, image->height};
    
    if(p[0] < image->width && p[0] >= 0 && p[1] < image->height && p[1] >= 0)
    {
		const unsigned int index = ((int)p[1] * image->width * image->bpp) + (int)p[0] * image->bpp;
		const unsigned int limit = image->width * image->height * image->bpp;
		color[0] = image->data[clampi(index,   0, limit)];
		color[1] = image->data[clampi(index+1, 0, limit)];
		color[2] = image->data[clampi(index+2, 0, limit)];
		color[3] = 255.f;
    }

    return color;
}
