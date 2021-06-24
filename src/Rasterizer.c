#include "../include/Rasterizer.h"
#include "../include/Vertex.h"
#include "../include/Endianness.h"

#include <limits.h>
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

static int fbfd = 0;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static int width, height;
static unsigned char* front_buffer = NULL;
static unsigned char* back_buffer = NULL;
static int clear_color = 100;
static float* depth_buffer;

static void (*vertex_shader)(struct Vertex*);
static void (*fragment_shader)(struct Vertex*, Color*);

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
            fragment_shader(vertex, &color);
            const long long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel * 0.125f) + (y+vinfo.yoffset) * finfo.line_length;
            *(back_buffer+location)     = (unsigned char)color[2];
            *(back_buffer+location + 1) = (unsigned char)color[1];
            *(back_buffer+location + 2) = (unsigned char)color[0];
            *(back_buffer+location + 3) = (unsigned char)255;
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

void tlSetShaders(void (*Vertex)(Vertex* vertex), void (*fragment)(struct Vertex*, Color* output))
{
    vertex_shader = Vertex;
    fragment_shader = fragment;
}

static void _tlDrawLine(Vertex* a, Vertex* b)
{
	int same_color;
    int i; 
    float x_inc, y_inc, z_inc, err_1, err_2;
    Vec3 point = a->pos;

    Vec3 d = b->pos - a->pos;

    x_inc = (d[0] < 0) ? -1 : 1;
    y_inc = (d[1] < 0) ? -1 : 1;
    z_inc = (d[2] < 0) ? -1 : 1;
	Vec3 lmn = _mm_abs_ps_2(d);
	Vec3 d2 = lmn * 2.f;
    d2[2] = lmn[2] * 2.f;
    Vertex vert;
    Color fragColor = {0};
    _tlDrawPoint(a);
    _tlDrawPoint(b);
    if ((lmn[0] >= lmn[1]) && (lmn[0] >= lmn[2])) 
    {
        err_1 = d2[1] - lmn[0];
        err_2 = d2[2] - lmn[0];
        for (i = 0; i < lmn[0]; i++) 
        {
            VertexInterpPtr(&vert, a, b, Distance(a->pos, point));
            fragment_shader(&vert, &fragColor);
            _tlDrawPoint(&vert);
            if (err_1 > 0) 
            {
                point[1] += y_inc;
                err_1 -= d2[0];
            }
            if (err_2 > 0)
            {
                point[2] += z_inc;
                err_2 -= d2[0];
            }
            err_1 += d2[1];
            err_2 += d2[2];
            point[0] += x_inc;
        }
    } 
    else if ((lmn[1] >= lmn[0]) && (lmn[1] >= lmn[2])) 
    {
        err_1 = d2[0] - lmn[1];
        err_2 = d2[2] - lmn[1];
        for (i = 0; i < lmn[1]; i++) 
        {
            VertexInterpPtr(&vert, a, b, Distance(a->pos, point));
            fragment_shader(&vert, &fragColor);
            _tlDrawPoint(&vert);
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= d2[1];
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= d2[1];
            }
            err_1 += d2[0];
            err_2 += d2[2];
            point[1] += y_inc;
        }
    } 
    else 
    {
        err_1 = d2[1] - lmn[2];
        err_2 = d2[0] - lmn[2];
        for (i = 0; i < lmn[2]; i++) 
        {
            VertexInterpPtr(&vert, a, b, Distance(a->pos, point));
            fragment_shader(&vert, &fragColor);
            _tlDrawPoint(&vert);

            if (err_1 > 0) 
            {
                point[1] += y_inc;
                err_1 -= d2[2];
            }
            if (err_2 > 0) 
            {
                point[0] += x_inc;
                err_2 -= d2[2];
            }
            err_1 += d2[1];
            err_2 += d2[0];
            point[2] += z_inc;
        }
    }
    fragment_shader(&vert, &fragColor);
    _tlDrawPoint(&vert);
}

void _tlDrawTriangle(Vertex* a, Vertex* b, Vertex* c)
{
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
    Vertex* vertices = (Vertex*)malloc(elements * sizeof(Vertex));
    unsigned int __elements = elements;

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
        memcpy(vertices, buffer, sizeof(Vertex) * elements);
        for(int i = 0; i < elements; ++i)
            vertex_shader(&vertices[i]);
    }

    switch(mode)
    {
        case POINTS:
        {
            for(int i = 0; i < __elements; ++i)
            {
                _tlDrawPoint(&vertices[i]);
            }
        }
        break;
        case LINES:
            for(unsigned int i = 0; i < __elements; i+=3)
            {
                _tlDrawLine(&vertices[i], &vertices[i+1]);
                _tlDrawLine(&vertices[i+1], &vertices[i+2]);
            }
        break;
        case LINES_STRIP:
            for(unsigned int i = 0; i < __elements; i++)
                _tlDrawLine(&vertices[i], &vertices[i+1]);
        break;
        case LINES_LOOP:
            for(unsigned int i = 0; i < __elements; i+=3)
            {
                _tlDrawLine(&vertices[i], &vertices[i+1]);
                _tlDrawLine(&vertices[i+1], &vertices[i+2]);
                _tlDrawLine(&vertices[i+2], &vertices[i]);
            }
        break;
        case TRIANGLES:
            for(unsigned int i = 0; i < __elements; i+=3)
                 _tlDrawTriangle(&vertices[i], &vertices[i+1], &vertices[i+2]);
        break;
        case TRIANGLES_LOOP:
            for(unsigned int i = 1; i < __elements; i+=2)
                    _tlDrawTriangle(&vertices[i-1], &vertices[i], &vertices[(i+1) % __elements]);
        break;
        case TRIANGLES_STRIP:
            for(unsigned int i = 1; i < __elements; i+=2)
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
