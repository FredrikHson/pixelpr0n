#include "burningsand.h"
#include "globals.h"
#include "fpscounter.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * pixel buffer of these grains
 * fire rules: if heat> ignition temperature add 10ish heat and
 * remove 1 remainingfuel
 * but only if there is free pixels somewhere around the burning particle
 * blur and move heat upwards as a regular fire effect
 * if the fuel goes to zero then the pixel is free air
 * particle movement rules just as regular sand
 * maybe try out to move really hot particles upwards
 */
struct sandgrain
{
    unsigned char heat;
    unsigned char remainingfuel;
    unsigned char color[3]; // the color of just the grain not the fire that is handled in a separate pass
    unsigned char frame; // to avoid moving a grain more than once per frame
};

sandgrain* sandbuffer = 0;
unsigned int sand_w = 0;
unsigned int sand_h = 0;

void init_burningSand()
{
    if(sand_w != (width + 2) || sand_h != (height))
    {
        destroyBurningSand();
        sand_w = width + 2; // +2 to avoid ifcases in the sand physics loop
        sand_h = height;
        sandbuffer = new sandgrain[sand_w * sand_h];
    }
}
void createnewfuelgrain()
{
    static float pos = 0;
    pos += deltatime * 5;

    if(pos > sand_w - 51)
    {
        pos = 0;
    }

    unsigned int offset = ((rand() % sand_w) + (rand() % 1) * sand_w);

    offset = pos + rand() % (50) + (rand() % 5) * sand_w;

    if(offset >= sand_w * sand_h)
    {
        offset = 0;
    }

    if(sandbuffer[offset].remainingfuel == 0)
    {
        unsigned char color = (int)(abstime * 255 + offset / 60) % 200;
        color += 55;

        sandbuffer[offset].remainingfuel = rand() % 55 + 200;
        sandbuffer[offset].heat = 0;
        sandbuffer[offset].color[0] = color;
        sandbuffer[offset].color[1] = color;
        sandbuffer[offset].color[2] = color;
    }
}
void updateSandPhysics()
{
    static unsigned char frame = 0;
    frame++;

    for(unsigned int i = 0; i < 50; i++)
    {
        createnewfuelgrain();
    }

    for(int y = sand_h - 2; y >= 0; y--)
    {
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 1; x < sand_w - 1; x++)
        {
            if(sandbuffer[sand_offset_y + sand_w + x].remainingfuel == 0)
            {
                sandbuffer[sand_offset_y + sand_w + x] = sandbuffer[sand_offset_y + x];
                sandbuffer[sand_offset_y + sand_w + x].frame = frame;
                sandbuffer[sand_offset_y + x] = {0};
            }
            else if(sandbuffer[sand_offset_y + sand_w + x + 1].remainingfuel == 0 &&
                    sandbuffer[sand_offset_y + x].frame != frame)
            {
                sandbuffer[sand_offset_y + sand_w + 1 + x] = sandbuffer[sand_offset_y + x];
                sandbuffer[sand_offset_y + sand_w + 1 + x].frame = frame;
                sandbuffer[sand_offset_y + x] = {0};
            }
        }
    }

    for(int y = sand_h - 2; y >= 0; y--)
    {
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 1; x < sand_w - 1; x++)
        {
            if(sandbuffer[sand_offset_y + sand_w + x - 1].remainingfuel == 0 &&
                    sandbuffer[sand_offset_y + x].frame != frame)
            {
                sandbuffer[sand_offset_y + sand_w - 1 + x] = sandbuffer[sand_offset_y + x];
                sandbuffer[sand_offset_y + sand_w - 1 + x].frame = frame;
                sandbuffer[sand_offset_y + x] = {0};
            }
        }
    }
}

void updateFire()
{

}

void drawBurningSand() // alpha stores the sand existance or not so even black sand can be used
{
    init_burningSand();
    static float delta = 0;
    delta += deltatime;

    if(delta > 1.0f / 240.0f)
    {
        delta = 0;
    }
    else
    {
        return;
    }

    updateSandPhysics();
    updateFire();


    for(unsigned int y = 0; y < height; y++)
    {
        unsigned int pixel_offset_y = y * width * 4;
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 0; x < width; x++)
        {
            pixels[pixel_offset_y + x * 4] = sandbuffer[sand_offset_y + x + 1].color[0];
            pixels[pixel_offset_y + x * 4 + 1] = sandbuffer[sand_offset_y + x + 1].color[1];
            pixels[pixel_offset_y + x * 4 + 2] = sandbuffer[sand_offset_y + x + 1].color[2];

        }
    }



    printFps();
}

void destroyBurningSand()
{

}
