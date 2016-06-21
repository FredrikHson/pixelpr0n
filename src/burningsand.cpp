#include "burningsand.h"
#include "globals.h"
#include "fpscounter.h"
#include <memory.h>
#include <stdlib.h>

void init_burningSand()
{
    static unsigned int sand_w = 0;
    static unsigned int sand_h = 0;

    if(sand_w != (width) || sand_h != (height))
    {
        destroyBurningSand();
        sand_w = width;
        sand_h = height;
        memset(pixels, 0, width * height * 4);
    }
}
void createnewfuelgrain()
{
    unsigned int rpix = ((rand() % width) + (rand() % 5) * width + width) * 4;

    if(rpix >= width * height * 4)
    {
        rpix = 0;
    }

    if(pixels[rpix + 3] == 0)
    {
        unsigned char color = (int)(abstime * 255 + rpix / 60) % 200;
        color += 55;
        pixels[rpix]        = color;
        pixels[rpix + 1]      = color;
        pixels[rpix + 2]      = color;
        pixels[rpix + 3]    = 1;
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

    for(unsigned int i = 0; i < 50; i++)
    {
        createnewfuelgrain();
    }

    for(unsigned int y = height - 2; y > 0; y--)
    {
        for(unsigned int x = 1; x < width - 1; x++)
        {
            unsigned int offset = (x + y * width) * 4;

            if(pixels[offset + 3] == 1)
            {
                unsigned int offset2 = offset + width * 4;

                if(pixels[offset2 + 3] == 0)
                {
                    pixels[offset2 + 0] = pixels[offset];
                    pixels[offset2 + 1] = pixels[offset + 1];
                    pixels[offset2 + 2] = pixels[offset + 2];
                    pixels[offset2 + 3] = 2;
                    pixels[offset] = 0;
                    pixels[offset + 1] = 0;
                    pixels[offset + 2] = 0;
                    pixels[offset + 3] = 0;
                }
            }
        }
    }

    for(unsigned int y = height - 2; y > 0; y--)
    {
        for(unsigned int x = width - 1; x > 2; x--)
        {
            unsigned int offset = (x + y * width) * 4;

            if(pixels[offset + 3] == 1)
            {
                unsigned int offset2 = offset + width * 4;

                int biasdirection = (rand() % 2) * 8 - 4;

                if(pixels[offset2 + 3 - biasdirection] == 0)
                {
                    pixels[offset2 + 0 - biasdirection] = pixels[offset];
                    pixels[offset2 + 1 - biasdirection] = pixels[offset + 1];
                    pixels[offset2 + 2 - biasdirection] = pixels[offset + 2];
                    pixels[offset2 + 3 - biasdirection] = 2;
                    pixels[offset] = 0;
                    pixels[offset + 1] = 0;
                    pixels[offset + 2] = 0;
                    pixels[offset + 3] = 0;
                }
                else if(pixels[offset2 + 3 + biasdirection] == 0)
                {
                    pixels[offset2 + 0 + biasdirection] = pixels[offset];
                    pixels[offset2 + 1 + biasdirection] = pixels[offset + 1];
                    pixels[offset2 + 2 + biasdirection] = pixels[offset + 2];
                    pixels[offset2 + 3 + biasdirection] = 2;
                    pixels[offset] = 0;
                    pixels[offset + 1] = 0;
                    pixels[offset + 2] = 0;
                    pixels[offset + 3] = 0;
                }
            }
        }
    }


    for(unsigned int i = 0; i < width * height; i++)
    {
        if(pixels[i * 4 + 3])
        {
            pixels[i * 4 + 3] = 1;
        }
    }



    printFps();
}

void destroyBurningSand()
{

}
