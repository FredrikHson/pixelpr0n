#include <stdio.h>
#include "fpscounter.h"
#include "globals.h"

void printFps()
{

    static float timesincelast = 0;
    static unsigned int frames = 0;
    timesincelast += deltatime;
    frames++;

    if(timesincelast > 2.0)
    {
        printf("%ifps\n", frames / 2);
        frames = 0;
        timesincelast = 0;
    }
}
