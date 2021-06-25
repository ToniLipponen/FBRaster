#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include "include/FBRaster.h"

Mat4x4 scalem;
Mat4x4 rotm;
Mat4x4 trans;

void vertex(Vertex* vertex)
{
    vertex->pos = MulVec4Mat4(vertex->pos, rotm);
    vertex->pos = MulVec4Mat4(vertex->pos, scalem);
    vertex->pos = MulVec4Mat4(vertex->pos, trans);
}

Image img;
#define discard_fragment return -1
int fragment(struct Vertex* vert, Color* output)
{
    *output = SampleImage(&img, vert->uv);
    *output *= fabsf(Vec4_Dot((Vec4){0,0,1,0}, vert->col));
}

int main()
{
    tlInitialize();
    tlEnable(BACKFACE_CULLING);
    tlEnable(CALCULATE_TRIANGLE_NORMALS);
    tlSetShaders(vertex, fragment);
    InitMouse();
    LoadImage("res/Wood_Box.png", &img);
    scalem = ScaleMat4X4(50, 50, 50);
    trans  = TranslateMat4X4((Vec4){960, 500, 10, 1});

	// There is no vec3 type
    const Vertex cube[] = 
    {
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1}, {0,0,0,0}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1}, {0,0,0,0}, {1.0f, 0.0f}}
    };
    
	float rot = 0;
    float zoom = 50.f;
    int mouseX = 1920/3;
	int mouseY = 1080/2;
	
	while(1)
    {
        mouse_event ev = MousePoll();
        mouseX += ev.xoffset;
        mouseY += ev.yoffset;
        if(ev.button == MOUSE_LEFT)
            zoom += 5.f;
        else if(ev.button == MOUSE_RIGHT)
			zoom -= 5.f;
        tlClear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    	
		trans = TranslateMat4X4((Vec4){mouseX, mouseY, 10, 0});
	    rotm = RotateMat4X4(rot*1.5f, rot+=1.f, 0);
        scalem = ScaleMat4X4(zoom, zoom, zoom);

        tlDrawBufferIndexed(TRIANGLES, cube, 0, 36);
        
		trans  = TranslateMat4X4((Vec4){1920/2.f, 1080/2.f, 20, 0});
		rotm   = RotateMat4X4(rot, 0, rot);
		scalem = ScaleMat4X4(50,50,50);

		tlDrawBufferIndexed(TRIANGLES, cube, 0, 36);
		tlSwapBuffers();
    }
    FreeImage(&img);
    tlDestroy();
}
