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
    unsigned char inited;
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
        vbuffer[i].inited = false;
    }
}

float distance(float x, float y, float x2, float y2)
{
    float dx = x - x2;
    float dy = y - y2;
    return sqrt(dx * dx + dy * dy);
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
        pix.distance = distance(p[0], p[1], x, y);
        pix.inited = true;

    }
}
void voronoiStepP(int stepover)
{
    for(int i = vb_h - 1; i >= 0 ; i--)
    {
        unsigned int yoffset = i * vb_w;

        for(int j = vb_w - 1; j >= 0; j--)
        {
            voronoi_pixel& p1 = vbuffer[j + yoffset];

            if(j + stepover < vb_w)
            {
                voronoi_pixel& p2 = vbuffer[j + stepover + yoffset];
                float dist = distance(j , i, p2.x, p2.y);

                if(dist < p1.distance)
                {
                    p1 = p2;
                    p1.distance = dist;
                }
            }

            if(i + stepover < vb_h)
            {
                voronoi_pixel& p2 = vbuffer[j + stepover * vb_w + yoffset];
                float dist = distance(j , i, p2.x, p2.y);

                if(dist < p1.distance)
                {
                    p1 = p2;
                    p1.distance = dist;
                }
            }
        }
    }

}

void voronoiStepN(int stepover)
{
    for(int i = 0; i < vb_h; i++)
    {
        unsigned int yoffset = i * vb_w;

        for(int j = 0; j < vb_w; j++)
        {
            voronoi_pixel& p1 = vbuffer[j + yoffset];

            if(j - stepover >= 0)
            {
                voronoi_pixel& p2 = vbuffer[j - stepover + yoffset];
                float dist = distance(j , i, p2.x, p2.y);

                if(dist < p1.distance)
                {
                    p1 = p2;
                    p1.distance = dist;
                }
            }

            if(i - stepover >= 0)
            {
                voronoi_pixel& p2 = vbuffer[j - stepover * vb_w + yoffset];
                float dist = distance(j , i, p2.x, p2.y);

                if(dist < p1.distance)
                {
                    p1 = p2;
                    p1.distance = dist;
                }
            }
        }
    }
}
void calculateVoronoi(int stepover = 8)
{

    voronoiStepP(stepover);
    voronoiStepN(stepover);

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

    // move and wrap
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
