#ifndef __DRAWSHAPES_H__
#define __DRAWSHAPES_H__

// Helper functions for dawing different shapes


void drawLine(unsigned int x1, unsigned int y1,
              unsigned int x2, unsigned int y2,
              unsigned char r, unsigned char g, unsigned char b);
// TODO: implement zbuffered lines
void drawLine(unsigned int x1, unsigned int y1, float z1,
              unsigned int x2, unsigned int y2, float z2,
              unsigned char r, unsigned char g, unsigned char b);

void drawTriangle(unsigned int x1, unsigned int y1, unsigned short z1,
                  unsigned int x2, unsigned int y2, unsigned short z2,
                  unsigned int x3, unsigned int y3, unsigned short z3,
                  unsigned char r, unsigned char g, unsigned char b);

#endif //__DRAWSHAPES_H__
