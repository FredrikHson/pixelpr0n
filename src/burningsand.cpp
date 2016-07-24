#include "burningsand.h"
#include "globals.h"
#include "fpscounter.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "fire.h"

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
        init_firepal();
    }
}
void createnewfuelgrain()
{
    static float  pos = 0;
    static int frames = 0;
    frames++;

    if(frames > 1500)
    {
        pos = rand() % sand_w;
        frames = 0;
    }

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
        //unsigned char color = (int)(abstime * 55 + offset / 60) % 50;
        //color += 25;
        unsigned char color = (int)(abstime * 55 + offset / 60) % 50;
        color += 25;

        sandbuffer[offset].remainingfuel = rand() % 55 + 100;
        sandbuffer[offset].heat = (((rand() % 2000)) / 1999) * 105;
        sandbuffer[offset].color[0] = color;
        sandbuffer[offset].color[1] = color / 2;
        sandbuffer[offset].color[2] = color / 3;
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
            if(sandbuffer[sand_offset_y + x].remainingfuel != 0)
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
                    sandbuffer[sand_offset_y + x].heat = {0};
                }
            }
        }
    }

    for(int y = sand_h - 2; y >= 0; y--)
    {
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 1; x < sand_w - 1; x++)
        {
            if(sandbuffer[sand_offset_y + x].remainingfuel != 0)
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
}

void updateFire()
{
    // remove fuel from hot grains and add more heat
    for(unsigned int y = 0; y < sand_h; y++)
    {
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 1; x < sand_w - 1; x++)
        {
            short fuel = sandbuffer[sand_offset_y + x].remainingfuel;

            if(fuel > 0)
            {
                unsigned char heat = sandbuffer[sand_offset_y + x].heat;

                if(heat > 50)
                {
                    if(heat > 200)
                    {
                        fuel -= 4;
                    }
                    else if(heat > 100)
                    {
                        fuel -= 2;
                    }
                    else
                    {
                        fuel -= 1;
                    }

                    if(fuel <= 0)
                    {
                        fuel = 0;
                        sandbuffer[sand_offset_y + x] = {0};
                    }
                    else
                    {

                        sandbuffer[sand_offset_y + x].remainingfuel = fuel;

                        if(sandbuffer[sand_offset_y + x].heat < 250)
                        {
                            sandbuffer[sand_offset_y + x].heat += 2;
                        }
                    }
                }
            }
            else if(sandbuffer[sand_offset_y + x].heat > 0)
            {
                sandbuffer[sand_offset_y + x].heat -= 1;
            }
        }
    }

    for(unsigned int y = 1; y < sand_h; y++)
    {
        unsigned int sand_offset_y = y * sand_w;

        for(unsigned int x = 1; x < sand_w - 1; x++)
        {
            unsigned int pos = sand_offset_y + x;
            unsigned short heat = 0;
            unsigned char oldheat = sandbuffer[pos].heat;
            heat += sandbuffer[pos].heat;
            heat += sandbuffer[pos - 1].heat;
            heat += sandbuffer[pos + 1].heat;
            heat += sandbuffer[pos - sand_w].heat * 2;
            heat += sandbuffer[pos + sand_w].heat * 6;
            heat /= 11;

            if(heat < oldheat)// && sandbuffer[pos].remainingfuel!=0)
            {
                heat = oldheat;
            }

            sandbuffer[pos].heat = heat;
        }
    }
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
            sandgrain* grain = &sandbuffer[sand_offset_y + x + 1];
            unsigned int pixel_offset = pixel_offset_y + x * 4;
            unsigned char c2[3] =
            {
                FirePal[grain->heat * 3],
                FirePal[grain->heat * 3 + 1],
                FirePal[grain->heat * 3 + 2]
            };
            pixels[pixel_offset]     = (int)(255 - (((255 - grain->color[0]) * (255 - c2[0])) / 255));
            pixels[pixel_offset + 1] = (int)(255 - (((255 - grain->color[0]) * (255 - c2[1])) / 255));
            pixels[pixel_offset + 2] = (int)(255 - (((255 - grain->color[0]) * (255 - c2[2])) / 255));
            //pixels[pixel_offset]     = grain->color[0];
            //pixels[pixel_offset + 1] = grain->color[0];
            //pixels[pixel_offset + 2] = grain->color[0];

        }
    }



    printFps();
}

void destroyBurningSand()
{

}
