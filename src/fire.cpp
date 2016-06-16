#include "fire.h"
#include "globals.h"
#include "fpscounter.h"
#include <stdlib.h>
#include <stdio.h>

unsigned int heatbuffer_w = 0;
unsigned int heatbuffer_h = 0;
unsigned char* heatbuffer = 0;
unsigned char FirePal[768];

inline unsigned char lerp(unsigned char x1, unsigned char x2, float p)
{
    return (unsigned char)((float)x1 * (1.0f - p) + (float)x2 * p);
}
void init_firepal()
{
    for(unsigned int i=0; i < 64; i++)
    {
        unsigned int offset = (i) * 3;
        FirePal[offset]     = lerp(0, 96, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(0, 96, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(0, 96, (float)i / 64.0f);
    }

    for(unsigned int i=0; i < 64; i++)
    {
        unsigned int offset = (i + 64) * 3;
        FirePal[offset]     = lerp(96, 255, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(96, 0, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(96, 0, (float)i / 64.0f);
    }

    for(unsigned int i=0; i < 64; i++)
    {
        unsigned int offset = (i + 128) * 3;
        FirePal[offset]     = lerp(255, 255, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(0, 255, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(0, 0, (float)i / 64.0f);
    }

    for(unsigned int i=0; i < 64; i++)
    {
        unsigned int offset = (i + 192) * 3;
        FirePal[offset]     = lerp(255, 255, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(255, 255, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(0, 255, (float)i / 64.0f);
    }
}
void init_heatbuffer()
{
    if(heatbuffer_w != (width + 2) || heatbuffer_h != (height + 1))
    {

        printf("resizing fire buffer\n");
        printf("old:%ix%i\n", heatbuffer_w, heatbuffer_w);
        destroyFire();
        heatbuffer_w = width + 2;
        heatbuffer_h = height + 1;
        printf("new:%ix%i\n", heatbuffer_w, heatbuffer_w);
        heatbuffer = new unsigned char[heatbuffer_w * heatbuffer_h];
        init_firepal();
    }
}
void destroyFire()
{
    if(heatbuffer)
    {
        delete [] heatbuffer;
        heatbuffer   = 0;
        heatbuffer_w = 0;
        heatbuffer_h = 0;
    }
}

void drawFire()
{
    init_heatbuffer();

    for(unsigned int i = 0; i < width * height; i++)
    {

        unsigned short color = i%width % 255;
        color *= 3;
        pixels[i * 4] = FirePal[color];
        pixels[i * 4 + 1] = FirePal[color + 1];
        pixels[i * 4 + 2] = FirePal[color + 2];
    }

    printFps();
}
