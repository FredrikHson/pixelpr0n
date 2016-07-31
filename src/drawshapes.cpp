#include "drawshapes.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

#define FBITS 16

void drawLine(unsigned int x1, unsigned int y1,
              unsigned int x2, unsigned int y2,
              unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int maxx = x2;
    unsigned int maxy = y2;
    unsigned int minx = x1;
    unsigned int miny = y1;

    if(minx > x2)
    {
        maxx = x1;
        maxy = y1;
        minx = x2;
        miny = y2;
    }

    int dx = maxx - minx;
    int dy = maxy - miny;

    int slope = 0;

    if(abs(dy) > dx)
    {
        minx = x1;
        miny = y1;
        maxx = x2;
        maxy = y2;

        if(miny > y2)
        {
            minx = x2;
            miny = y2;
            maxy = y1;
            maxx = x1;
        }

        // bitshift everything for fixedpoint math
        int dx = maxx - minx;
        int dy = maxy - miny;

        if(dy != 0)
        {
            slope = (dx << FBITS) / dy;
        }


        int offsetx = 0;
        unsigned int offsety = minx * 4 + miny * width * 4;

        for(int i = miny; i < maxy; i++)
        {
            if(i > width || i < 0)
            {
                return;
            }

            offsetx += slope;
            offsety += width * 4;
            unsigned int offset = ((offsetx >> FBITS) * 4) + offsety;

            if(offset >= width * height * 4)
            {
                return;
            }

            pixels[offset] = r;
            pixels[offset + 1] = g;
            pixels[offset + 2] = b;
        }
    }
    else
    {

        if(dx != 0)
        {
            slope = (dy << FBITS) / dx;
        }

        unsigned int offsetx = minx * 4;
        int offsety = miny << FBITS;

        for(int i = minx; i < maxx; i++)
        {
            offsetx += 4;
            offsety += slope;
            unsigned int offset = offsetx  + (offsety >> FBITS) * width * 4;

            if(offset >= width * height * 4)
            {
                return;
            }

            pixels[offset] = r;
            pixels[offset + 1] = g;
            pixels[offset + 2] = b;
        }
    }
}

void drawLine(unsigned int x1, unsigned int y1, float z1,
              unsigned int x2, unsigned int y2, float z2,
              unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int maxx = x2;
    unsigned int maxy = y2;
    unsigned int minx = x1;
    unsigned int miny = y1;
    float maxz = z2;
    float minz = z1;

    if(minx > x2)
    {
        maxx = x1;
        maxy = y1;
        maxz = z1;
        minx = x2;
        miny = y2;
        minz = z2;
    }

    int dx = maxx - minx;
    int dy = maxy - miny;
    float dz = maxz - minz;

    int slope = 0;

    if(abs(dy) > dx)
    {
        maxx = x2;
        maxy = y2;
        maxz = z2;
        minx = x1;
        miny = y1;
        minz = z1;

        if(miny > y2)
        {
            maxx = x1;
            maxy = y1;
            maxz = z1;
            minx = x2;
            miny = y2;
            minz = z2;
        }

        // bitshift everything for fixedpoint math
        int dx = maxx - minx;
        int dy = maxy - miny;
        float dz = maxz - minz;

        if(dy != 0)
        {
            dz = dz / (float)dy;
            slope = (dx << FBITS) / dy;
        }


        int offsetx = 0;
        unsigned int offsety = minx * 4 + miny * width * 4;
        float z=minz;
        for(int i = miny; i < maxy; i++)
        {
            if(i > width || i < 0)
            {
                return;
            }

            offsetx += slope;
            offsety += width * 4;
            unsigned int offset = ((offsetx >> FBITS) * 4) + offsety;

            if(offset >= width * height * 4)
            {
                return;
            }

            if(zbuffer[offset >> 2] > z)
            {
                zbuffer[offset >> 2] = z;
            pixels[offset] = r;
            pixels[offset + 1] = g;
            pixels[offset + 2] = b;
            z+=dz;
            }
        }
    }
    else
    {

        if(dx != 0)
        {
            dz = dz / (float)dx;
            slope = (dy << FBITS) / dx;
        }

        unsigned int offsetx = minx * 4;
        int offsety = miny << FBITS;
        float z = minz;

        for(int i = minx; i < maxx; i++)
        {
            offsetx += 4;
            offsety += slope;
            unsigned int offset = offsetx  + (offsety >> FBITS) * width * 4;

            if(offset >= width * height * 4)
            {
                return;
            }

            if(zbuffer[offset >> 2] > z)
            {
                zbuffer[offset >> 2] = z;
                pixels[offset] = r;
                pixels[offset + 1] = g;
                pixels[offset + 2] = b;
            }

            z += dz;
        }
    }
}


void drawTriangle(unsigned int x1, unsigned int y1, unsigned short z1,
                  unsigned int x2, unsigned int y2, unsigned short z2,
                  unsigned int x3, unsigned int y3, unsigned short z3,
                  unsigned char r, unsigned char g, unsigned char b)
{

}
