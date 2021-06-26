#include "../include/Rasterizer.h"
#include "../include/Vertex.h"
#include "../include/Endianness.h"

#include <limits.h>
#include <pmmintrin.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdalign.h>
#include <memory.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


static unsigned int settings_int;
static int fbfd = 0;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static int width, height;
static unsigned char* front_buffer = NULL;
static unsigned char* back_buffer = NULL;
static int clear_color = 0x44444444;
static float* depth_buffer;

static void (*vertex_shader)(struct Vertex*);
static int  (*fragment_shader)(struct Vertex*, Color*);

void tlClear(unsigned int mask)
{
    if(mask & COLOR_BUFFER_BIT)
        for(int i = 0; i < height*width; ++i)
            ((int*)back_buffer)[i] = clear_color;
    if(mask & DEPTH_BUFFER_BIT)
        memset(depth_buffer, 9999999, width*height*4);
}

void tlClearColor(int c)
{
	clear_color = c;
}

void tlClearColorRGB(unsigned char r, unsigned char g, unsigned char b)
{
    union Convert{ int hex; unsigned char rgb[4]; };
    static union Convert converter;
    converter.rgb[0] = 0;
    converter.rgb[1] = r;
    converter.rgb[2] = g;
    converter.rgb[3] = b;
    clear_color = converter.hex;
}

static void _tlDrawPoint(struct Vertex* vertex)
{
    const int x = (int)vertex->pos[0];
    const int y = (int)vertex->pos[1];
    if(x >= 0 && x < width && y >= 0 && y < height)
    {
        const int index = y * width + x;
        if(vertex->pos[2] <= depth_buffer[index])
        {
            Color color = {0};
            if(fragment_shader(vertex, &color) == -1)
				return;
            const long long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel * 0.125f) + (y+vinfo.yoffset) * finfo.line_length;
            *(back_buffer+location)     = (unsigned char)color[2];
            *(back_buffer+location + 1) = (unsigned char)color[1];
            *(back_buffer+location + 2) = (unsigned char)color[0];
            *(back_buffer+location + 3) = (unsigned char)255-color[3];
            depth_buffer[index] = vertex->pos[2];
        }
    }
}

void tlInitialize()
{
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    width = vinfo.xres;
    height = vinfo.yres;
    front_buffer = (unsigned char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    back_buffer  = (unsigned char*)malloc(screensize);
    depth_buffer = (float*)malloc(width*height*sizeof(float));
    tlClear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
}

void tlEnable(unsigned int setting)
{
    settings_int |= (1 << (setting - 1));
}

void tlDisable(unsigned int setting)
{
    settings_int &= ~(1 << (setting - 1));
}

void tlSetShaders(void (*Vertex)(Vertex* vertex), int (*fragment)(struct Vertex*, Color* output))
{
    vertex_shader = Vertex;
    fragment_shader = fragment;
}

static void _tlDrawLine(Vertex* a, Vertex* b)
{
    const float db = Distance(a->pos, b->pos);
    Vertex vertex;
    for(unsigned int i = 0; i < db; ++i)
    {
        if(VertexInterpPtr(&vertex, a, b, i) >= 1.f)
            return;
        _tlDrawPoint(&vertex);
    }
}

void _tlDrawTriangle(Vertex* a, Vertex* b, Vertex* c)
{
    if(settings_int & BACKFACE_CULLING)
    {
        if(Vec4_Dot(a->col, (Vec4){0.f,0.f,1.f,0.f}) >= 0.0)
            return;
    }

	const float db = Distance(b->pos,c->pos);
    Vertex A, B;
    for(unsigned int i = 0; i < db; ++i)
    {
        if(VertexInterpPtr(&A, c, a, i) >= 1.f)
			return;
        VertexInterpPtr(&B, c, b, i);
        _tlDrawLine(&A, &B);
    }
}

void tlDrawBufferIndexed(unsigned int mode, Vertex *buffer, unsigned int* index_buffer, unsigned int elements)
{
    if(!buffer || elements == 0)
        return;
    Vertex* vertices = (Vertex*)aligned_alloc(alignof(Vertex), elements * sizeof(Vertex));
   
    if(index_buffer)
    {
		for(int i = 0; i < elements; ++i)
		{
            vertices[i] = buffer[index_buffer[i]];
            vertex_shader(&vertices[i]);
		}
	}
    else
    {
        for(int i = 0; i < elements; ++i)
        {
            vertices[i] = buffer[i];
            vertex_shader(&vertices[i]);
        }
    }
    if(settings_int & CALCULATE_TRIANGLE_NORMALS)
    {
        for(int i = 0; i < elements; i += 3)
        {
            const Vec4 v1 = vertices[i + 1].pos - vertices[i].pos;
            const Vec4 v2 = vertices[i + 2].pos - vertices[i].pos;
            const Vec4 n  = Normalize(Vec4_Cross(v1, v2));
            vertices[i].col = n;
            vertices[i+1].col = n;
            vertices[i+2].col = n;
        }
    }

    switch(mode)
    {
        case POINTS:
        {
            for(int i = 0; i < elements; ++i)
            {
                _tlDrawPoint(&vertices[i]);
            }
        }
        break;
        case LINES:
            for(unsigned int i = 0; i < elements; i+=3)
            {
                _tlDrawLine(&vertices[i], &vertices[i+1]);
                _tlDrawLine(&vertices[i+1], &vertices[i+2]);
            }
        break;
        case LINES_STRIP:
            for(unsigned int i = 0; i < elements; i++)
                _tlDrawLine(&vertices[i], &vertices[i+1]);
        break;
        case LINES_LOOP:
            for(unsigned int i = 0; i < elements; i+=3)
            {
                _tlDrawLine(&vertices[i], &vertices[i+1]);
                _tlDrawLine(&vertices[i+1], &vertices[i+2]);
                _tlDrawLine(&vertices[i+2], &vertices[i]);
            }
        break;
        case TRIANGLES:
            for(unsigned int i = 0; i < elements; i+=3)
                 _tlDrawTriangle(&vertices[i], &vertices[i+1], &vertices[i+2]);
        break;
        case TRIANGLES_LOOP:
            for(unsigned int i = 1; i < elements; i+=2)
                _tlDrawTriangle(&vertices[i-1], &vertices[i], &vertices[(i+1) % elements]);
        break;
        case TRIANGLES_STRIP:
            for(unsigned int i = 1; i < elements; i+=2)
                _tlDrawTriangle(&vertices[i-1], &vertices[i], &vertices[i+1]);
		break;
        default:
        break;
    }
    free(vertices);
}

void tlSwapBuffers()
{
    memcpy(front_buffer, back_buffer, screensize);
}

void tlDestroy()
{
    munmap(front_buffer, screensize);
    free(back_buffer);
    free(depth_buffer);
    close(fbfd);
}
