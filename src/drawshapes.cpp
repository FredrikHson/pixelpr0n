#include "drawshapes.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

#define FBITS 16

void drawLine(unsigned int x1, unsigned int y1,
              unsigned int x2, unsigned int y2,
              unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int minx = x1;
    unsigned int miny = y1;
    unsigned int maxx = x2;
    unsigned int maxy = y2;

    if(minx > x2)
    {
        minx = x2;
        miny = y2;
        maxy = y1;
        maxx = x1;
    }

    int dx = maxx - minx;
    int dy = maxy - miny;

    int slope = 0;

    if(abs(dy) > dx)
    {

        if(dy != 0)
        {
            slope = (dx << 16) / dy;
        }

        unsigned int offsetx = minx * 4;
        int offsety = miny << FBITS;

        for(int i = minx; i < maxx; i++)
        {
            offsetx += 4;
            offsety += slope;
            unsigned int offset = offsetx + ((offsety >> 16) * width * 4);

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

        if(dx != 0)
        {
            slope = (dy << 16) / dx;
        }

        int offsetx = 0;
        unsigned int offsety = minx * 4 + miny * width * 4;

        for(int i = miny; i < maxy; i++)
        {
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
}
