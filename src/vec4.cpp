#include "vec4.h"

float& vec4::operator[](size_t idx)
{
    return v[idx];
}
const float& vec4::operator[](size_t idx) const
{
    return v[idx];
}

vec4 vec4::operator/(const float& s)
{
    vec4 out;
    out[0] = v[0] / s;
    out[1] = v[1] / s;
    out[2] = v[2] / s;
    out[3] = v[3] / s;
    return out;
}

vec4 vec4::operator*(const float& s)
{
    vec4 out;
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
    out[3] = v[3] * s;
    return out;
}

void vec4::set(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1;
}
void vec4::divw()
{
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    v[3] = 1;
}
vec4::vec4( float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1;

}
