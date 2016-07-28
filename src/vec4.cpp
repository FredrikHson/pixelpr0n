#include "vec4.h"

float& vec4::operator[](size_t idx)
{
    return v[idx];
}
const float& vec4::operator[](size_t idx) const
{
    return v[idx];
}
void vec4::set(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1;
}
