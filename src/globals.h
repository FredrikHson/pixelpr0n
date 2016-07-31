#ifndef __GLOBALS_H__
#define __GLOBALS_H__

class matrix;

extern unsigned int width;
extern unsigned int height;
extern unsigned char* pixels;
extern float* zbuffer;
extern float deltatime;
extern float abstime;
extern matrix perspective;
extern matrix view;

#endif //__GLOBALS_H__
