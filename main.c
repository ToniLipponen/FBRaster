#include <stdio.h>
#include <unistd.h>
#include "include/Rasterizer.h"
#include "include/Utility.h"
#include "include/Matrix.h"
#include "include/Input.h"

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
void fragment(struct Vertex* vert, Color* output)
{
    *output = SampleImage(&img, vert->uv);
}

int main()
{
    tlInitialize();
    tlSetShaders(vertex, fragment);
    InitMouse();

    LoadImage("res/box_128x128.jpeg", &img);
    scalem = ScaleMat4X4(50, 50, 50);
    trans  = TranslateMat4X4((Vec3){960, 500, 10});

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
    const unsigned int cubeIndices[] = {
		/*front*/
		0, 1, 2,
		2, 3, 0,
		/*top*/
		1, 5, 6,
		6, 2, 1,
		/*back*/
		7, 6, 5,
		5, 4, 7,
		/*bottom*/
		4, 0, 3,
		3, 7, 4,
		/*left*/
		4, 5, 1,
		1, 0, 4,
		/*right*/
		3, 2, 6,
		6, 7, 3
	};
	float rot = 0;
    float zoom = 50.f;
    int mouseX = 1920/2;
	int mouseY = 1080/2;
    tlClearColorRGB(255, 255, 0);

	while(1)
    {
        mouse_event ev = MousePoll();
        mouseX += ev.xoffset;
        mouseY += ev.yoffset;
        if(ev.button == MOUSE_LEFT)
            scalem = ScaleMat4X4(zoom += 5.f, zoom, zoom);
        else if(ev.button == MOUSE_RIGHT)
            scalem = ScaleMat4X4(zoom -= 5.f, zoom, zoom);

        tlClear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    	trans = TranslateMat4X4((Vec3){mouseX, mouseY, 10});
	    rotm = RotateMat4X4(rot*1.5f, rot+=0.5f, 0);

        tlDrawBufferIndexed(TRIANGLES, cube, 0, 36);
        trans = TranslateMat4X4((Vec3){1920/2.f, 1080/2.f, 20});
		rotm  = RotateMat4X4(rot, 0, rot);
		tlDrawBufferIndexed(TRIANGLES, cube, 0, 36);

		tlSwapBuffers();
    }
    FreeImage(&img);
    tlDestroy();
}
