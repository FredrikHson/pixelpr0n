#include "fire.h"
#include "globals.h"
#include <stdlib.h>
#include "fpscounter.h"

void drawFire()
{

    for(unsigned int i = 0; i < 320 * 240; i++)
    {

        pixels[i * 4] = rand() % 255;
        pixels[i * 4 + 1] = rand() % 255;
        pixels[i * 4 + 2] = rand() % 255;
    }
    printFps();
}
