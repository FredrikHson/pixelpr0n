#ifndef __VEC4_H__
#define __VEC4_H__
#include <stdlib.h>

class vec4
{
    public:
    float v[4];

    float& operator[](size_t idx);
    const float& operator[](size_t idx) const;

    void set( float x, float y, float z);

};

#endif //__VEC4_H__
