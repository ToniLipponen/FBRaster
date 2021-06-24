#include "../include/Input.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static mouse_event event;
static int devID, bytesM;

int InitMouse()
{
    devID = open("/dev/input/mice", O_RDONLY | O_NONBLOCK);
    if(devID == -1){
        printf("Could not open mouse device\n");
        return 0;
    }
    return 1;
}

void CloseMouse()
{
    close(devID);
}

mouse_event MousePoll()
{
	event.xoffset = 0;
	event.yoffset = 0;
    bytesM = read(devID, &event, sizeof(event));
    event.yoffset *= -1;
    return event;
}