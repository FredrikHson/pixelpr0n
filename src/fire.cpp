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
    for(unsigned int i = 0; i < 64; i++)
    {
        unsigned int offset = (i) * 3;
        FirePal[offset]     = lerp(0, 96, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(0, 96, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(0, 96, (float)i / 64.0f);
    }

    for(unsigned int i = 0; i < 64; i++)
    {
        unsigned int offset = (i + 64) * 3;
        FirePal[offset]     = lerp(96, 255, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(96, 0, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(96, 0, (float)i / 64.0f);
    }

    for(unsigned int i = 0; i < 64; i++)
    {
        unsigned int offset = (i + 128) * 3;
        FirePal[offset]     = lerp(255, 255, (float)i / 64.0f);
        FirePal[offset + 1] = lerp(0, 255, (float)i / 64.0f);
        FirePal[offset + 2] = lerp(0, 0, (float)i / 64.0f);
    }

    for(unsigned int i = 0; i < 64; i++)
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

    heatbuffer[rand() % (heatbuffer_w * heatbuffer_h)] = 255;

    for(int i = 0; i < heatbuffer_w; i++)
    {
        unsigned int o = heatbuffer_w * (heatbuffer_h - 1) +i;
        if(heatbuffer[o]>7)
        heatbuffer[o] -= 8;

    }
    for(int i = 0; i < heatbuffer_w/10; i++)
    {
        unsigned int o = heatbuffer_w * (heatbuffer_h - 1) + rand() % heatbuffer_w;
        heatbuffer[o] = (rand() % 55) + 200;
    }

    for(unsigned int y = 0; y < heatbuffer_h; y++)
    {
        heatbuffer[0 + y * heatbuffer_w] = 0;
        heatbuffer[heatbuffer_w - 1 + y * heatbuffer_w] = 0;
    }

    for(unsigned int y = 0; y < heatbuffer_h - 1; y++)
    {

        for(unsigned int x = 1; x < heatbuffer_w - 1; x++)
        {
            unsigned int pos = x + y * heatbuffer_w;
            unsigned short heat = 0;
            heat += heatbuffer[pos];
            heat += heatbuffer[pos - 1];
            heat += heatbuffer[pos + 1];
            heat += heatbuffer[pos + heatbuffer_w] * 6;
            heatbuffer[pos] = heat / 9;

        }
    }

    // copy the heatbuffer to the backbuffer
    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            unsigned short color = heatbuffer[x + 1 + y * heatbuffer_w];
            color *= 3;
            unsigned int offset = (x + y * width) * 4;
            pixels[offset] = FirePal[color];
            pixels[offset + 1] = FirePal[color + 1];
            pixels[offset + 2] = FirePal[color + 2];
        }
    }

    printFps();
}
