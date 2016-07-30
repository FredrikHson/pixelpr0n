#include "matrix.h"
#include "vec4.h"
#include <math.h>
#include <stdio.h>

#define RAD M_PI / 180.0f

void matrix::setIdentity()
{
    mat[0]  = 1;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;

    mat[4]  = 0;
    mat[5]  = 1;
    mat[6]  = 0;
    mat[7]  = 0;

    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = 1;
    mat[11] = 0;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
}
void matrix::setScale(float x, float y, float z)
{
    mat[0]  = x;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;

    mat[4]  = 0;
    mat[5]  = y;
    mat[6]  = 0;
    mat[7]  = 0;

    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = z;
    mat[11] = 0;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
}

void matrix::setScale(float s)
{
    mat[0]  = s;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;

    mat[4]  = 0;
    mat[5]  = s;
    mat[6]  = 0;
    mat[7]  = 0;

    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = s;
    mat[11] = 0;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
}

void matrix::setRot(float angle, float x, float y, float z)
{
    setRotR(angle * RAD, x, y, z);
}
void matrix::setRotR(float angle, float x, float y, float z)
{
    float l = sqrt(x * x + y * y + z * z);
    float x2 = x / l;
    float y2 = y / l;
    float z2 = z / l;
    float s = sin(angle);
    float c = cos(angle);
    float ic = 1.0f - c;

    mat[0]  = (ic * x2 * x2) + c;
    mat[1]  = (ic * x2 * y2) - z2 * s;
    mat[2]  = (ic * z2 * x2) + y2 * s;
    mat[3]  = 0.0f;

    mat[4]  = (ic * x2 * y2) + z2 * s;
    mat[5]  = (ic * y2 * y2) + c;
    mat[6]  = (ic * y2 * z2) - x2 * s;
    mat[7]  = 0.0f;

    mat[8]  = (ic * z2 * x2) - y2 * s;
    mat[9]  = (ic * y2 * z2) + x2 * s;
    mat[10] = (ic * z2 * z2) + c;
    mat[11] = 0.0f;

    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = 0.0f;
    mat[15] = 1.0f;
}
void matrix::setTranslation(float x, float y, float z)
{
    mat[0]  = 1;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;

    mat[4]  = 0;
    mat[5]  = 1;
    mat[6]  = 0;
    mat[7]  = 0;

    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = 1;
    mat[11] = 0;

    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
    mat[15] = 1;
}

matrix matrix::operator*(const matrix& m2)
{
    matrix out;
    out[0] = mat[0] * m2[0]   + mat[1] * m2[4]  + mat[2] * m2[8]   + mat[3] * m2[12];
    out[1] = mat[0] * m2[1]   + mat[1] * m2[5]  + mat[2] * m2[9]   + mat[3] * m2[13];
    out[2] = mat[0] * m2[2]   + mat[1] * m2[6]  + mat[2] * m2[10]  + mat[3] * m2[14];
    out[3] = mat[0] * m2[3]   + mat[1] * m2[7]  + mat[2] * m2[11]  + mat[3] * m2[15];

    out[4] = mat[4] * m2[0]   + mat[5] * m2[4]  + mat[6] * m2[8]   + mat[7] * m2[12];
    out[5] = mat[4] * m2[1]   + mat[5] * m2[5]  + mat[6] * m2[9]   + mat[7] * m2[13];
    out[6] = mat[4] * m2[2]   + mat[5] * m2[6]  + mat[6] * m2[10]  + mat[7] * m2[14];
    out[7] = mat[4] * m2[3]   + mat[5] * m2[7]  + mat[6] * m2[11]  + mat[7] * m2[15];

    out[8] = mat[8] * m2[0]   + mat[9] * m2[4]  + mat[10] * m2[8]  + mat[11] * m2[12];
    out[9] = mat[8] * m2[1]   + mat[9] * m2[5]  + mat[10] * m2[9]  + mat[11] * m2[13];
    out[10] = mat[8] * m2[2]  + mat[9] * m2[6]  + mat[10] * m2[10] + mat[11] * m2[14];
    out[11] = mat[8] * m2[3]  + mat[9] * m2[7]  + mat[10] * m2[11] + mat[11] * m2[15];

    out[12] = mat[12] * m2[0] + mat[13] * m2[4] + mat[14] * m2[8]  + mat[15] * m2[12];
    out[13] = mat[12] * m2[1] + mat[13] * m2[5] + mat[14] * m2[9]  + mat[15] * m2[13];
    out[14] = mat[12] * m2[2] + mat[13] * m2[6] + mat[14] * m2[10] + mat[15] * m2[14];
    out[15] = mat[12] * m2[3] + mat[13] * m2[7] + mat[14] * m2[11] + mat[15] * m2[15];

    return out;
}
vec4 matrix::operator*(const vec4& v)
{
    vec4 out;

    out[0] = v[0] * mat[0]  + v[1] * mat[4]  + v[2] * mat[8]  + v[3] * mat[12];
    out[1] = v[0] * mat[1]  + v[1] * mat[5]  + v[2] * mat[9]  + v[3] * mat[13];
    out[2] = v[0] * mat[2]  + v[1] * mat[6]  + v[2] * mat[10] + v[3] * mat[14];
    out[3] = v[0] * mat[3] + v[1] * mat[7] + v[2] * mat[11] + v[3] * mat[15];

    return out;
}

void matrix::rotate(float angle, float x, float y, float z)
{
    matrix rot;
    rot.setRot(angle, x, y, z);
    *this = *this * rot;
}
void matrix::scale(float x, float y, float z)
{
    matrix scale;
    scale.setScale(x, y, z);
    *this = *this * scale;
}

void matrix::scale(float s)
{
    matrix scale;
    scale.setScale(s);
    *this = *this * scale;
}

void matrix::translate(float x, float y, float z)
{
    matrix translation;
    translation.setTranslation(x, y, z);
    *this = *this * translation;

}
void matrix::setPerspective(float fov, float aspect, float near, float far)
{
    float f = 1.0f / tan(fov / 2.0f);

    mat[0]  = f / aspect;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;

    mat[4]  = 0;
    mat[5]  = f;
    mat[6]  = 0;
    mat[7]  = 0;

    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = (far + near) / (near - far);
    mat[11] = (2 * far * near) / (near - far);

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = -1;
    mat[15] = 0;
}
float& matrix::operator[](size_t idx)
{
    return mat[idx];
}
const float& matrix::operator[](size_t idx) const
{
    return mat[idx];
}

void matrix::print()
{
    printf("matrix:\n"
           "\t%8.5f %8.5f %8.5f %8.5f\n"
           "\t%8.5f %8.5f %8.5f %8.5f\n"
           "\t%8.5f %8.5f %8.5f %8.5f\n"
           "\t%8.5f %8.5f %8.5f %8.5f\n",
           mat[0], mat[1], mat[2], mat[3],
           mat[4], mat[5], mat[6], mat[7],
           mat[8], mat[9], mat[10], mat[11],
           mat[12], mat[13], mat[14], mat[15]
          );
}
