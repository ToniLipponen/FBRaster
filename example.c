#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "include/FBRaster.h"
#include "include/Matrix.h"
#include "include/Vector.h"

Mat4x4 scalem;
Mat4x4 rotm;
Mat4x4 trans;

void vertex(Vertex* vertex)
{
    vertex->pos = MulVec4Mat4(vertex->pos, scalem);
    vertex->pos = MulVec4Mat4(vertex->pos, rotm);
    vertex->pos = MulVec4Mat4(vertex->pos, trans);
}

Image img;
int fragment(struct Vertex* vert, Color* output)
{
    *output = SampleImage(&img, vert->uv);
    *output *= fabsf(Vec4_Dot((Vec4){0,0,1,0}, vert->col));
    return 1;
}

int main()
{
    tlInitialize();
    tlSetShaders(vertex, fragment);
    LoadImage("res/Wood_Box.png", &img);

    Vertex cube[] = 
    {
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f,1.0f}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f,1.0f}, {0,0,0,0}, {1.0f, 0.0f}}
    };

    rotm   = RotateMat4X4(0, 45, 0);
    scalem = ScaleMat4X4(800,300,300);
    trans  = TranslateMat4X4((Vec4){1920/2.f, 1080/2.f, 200, 1}); 
    float rot = 0;    
	  while(1)
    {
		    rotm = RotateMat4X4(rot, rot, 0);
        rot += 0.5f;
        tlClear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
		    tlDrawBuffer(TRIANGLES, cube, 36);
		    tlSwapBuffers();
    }
	
    FreeImage(&img);
    tlDestroy();
}
