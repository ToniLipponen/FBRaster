#ifndef SIMPLE_INPUT
#define SIMPLE_INPUT

typedef struct mouse_event
{
	char button;
	char xoffset;
	char yoffset;
}mouse_event;

#define MOUSE_NULL_EVENT    8
#define MOUSE_LEFT          9
#define MOUSE_RIGHT         10
#define MOUSE_MIDDLE        12

int         InitMouse();
void        CloseMouse();
mouse_event MousePoll();

#endif
