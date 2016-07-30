#ifndef __VEC4_H__
#define __VEC4_H__
#include <stdlib.h>

class vec4
{
    public:
    float v[4];

    float& operator[](size_t idx);
    const float& operator[](size_t idx) const;
    vec4 operator/(const float& s);
    vec4 operator*(const float& s);

    void set( float x, float y, float z);
    void divw();
    vec4( float x, float y, float z);
    vec4(){};

};

#endif //__VEC4_H__
