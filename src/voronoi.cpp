#include "voronoi.h"
#include "globals.h"
#include "fpscounter.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec4.h"

struct voronoi_pixel
{
    float distance;
    float x, y; // closest point
    unsigned char r, g, b;
    unsigned char pad;
};

voronoi_pixel* vbuffer = 0;
unsigned short vb_w    = 0;
unsigned short vb_h    = 0;

#define NUM_VORONOI_POINTS 500

vec4 voronoi_input_points[NUM_VORONOI_POINTS];

void initVBuffer()
{
    vb_h = height;
    vb_w = width;

    if(vbuffer != 0)
    {
        delete [] vbuffer;
    }

    vbuffer = new voronoi_pixel[vb_h * vb_w];


}
void clearVBuffer()
{
    for(int i = 0; i < vb_h * vb_w; i++)
    {
        vbuffer[i].distance = __FLT_MAX__;
        vbuffer[i].x = __FLT_MAX__;
        vbuffer[i].y = __FLT_MAX__;
        vbuffer[i].r = 0;
        vbuffer[i].g = 0;
        vbuffer[i].b = 0;
    }
}
void initVBufferFromPoints(vec4* points, size_t numPoints)
{
    if(points == nullptr)
    {
        return;
    }

    srand(0);

    for(int i = 0; i < numPoints; i++)
    {
        vec4& p = points[i];
        int x = (int)p[0];
        int y = (int)p[1];

        if(x < 0 || x >= vb_w || y < 0 || y >= vb_h) // clipping
        {
            continue;
        }

        voronoi_pixel& pix = vbuffer[x + y * vb_w];


        pix.r = rand() % 255;
        pix.g = rand() % 255;
        pix.b = rand() % 255;

        pix.x = p[0];
        pix.y = p[1];
        float dx = p[0] - (float)x;
        float dy = p[1] - (float)y;
        pix.distance = sqrt(dx * dx + dy * dy);

    }
}
void calculateVoronoi(int stepover = 8)
{

    for(int i = 0; i < vb_h; i++)
    {
        unsigned int y_offset = i * vb_w;

        for(int j = 0; j < vb_w; j++)
        {

        }
    }

    if(stepover > 1)
    {
        calculateVoronoi(stepover / 2);
    }

}
void drawVoronoi()
{
    if(vb_h != height || vb_w != width || vbuffer == nullptr)
    {
        initVBuffer();

        for(int i = 0; i < NUM_VORONOI_POINTS; i++)
        {
            vec4& p = voronoi_input_points[i];
            p[0] = (float)(rand() % (vb_w * 100)) / 100;
            p[1] = (float)(rand() % (vb_h * 100)) / 100;
            // velocity
            p[2] = ((float)(rand() % (200)) / 100.0f - 1.0f) * 20.0f;
            p[3] = ((float)(rand() % (200)) / 100.0f - 1.0f) * 20.0f;
        }

    }

    for(int i = 0; i < NUM_VORONOI_POINTS; i++)
    {
        vec4& p = voronoi_input_points[i];
        p[0] += p[2] * deltatime;
        p[1] += p[3] * deltatime;

        if(p[0] < 0)
        {
            p[0] += vb_w;
        }

        if(p[0] > vb_w)
        {
            p[0] -= vb_w;
        }

        if(p[1] < 0)
        {
            p[1] += vb_h;
        }

        if(p[1] > vb_h)
        {
            p[1] -= vb_h;
        }
    }

    clearVBuffer();
    initVBufferFromPoints(voronoi_input_points, NUM_VORONOI_POINTS);
    calculateVoronoi();

    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            voronoi_pixel& vp = vbuffer[x + y * vb_w];

            unsigned int offset = (x + y * width) * 4;
            pixels[offset]      = vp.r;
            pixels[offset + 1]  = vp.g;
            pixels[offset + 2]  = vp.b;
        }
    }

    printFps();
}

void destroyVoronoi()
{
    if(vbuffer != 0)
    {
        delete [] vbuffer;
    }
}
