#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdlib.h>

class vec4;

class matrix
{
public:
    float mat[16];

    matrix operator*(const matrix& m2);
    vec4 operator*(const vec4& v);
    float& operator[](size_t idx);
    const float& operator[](size_t idx) const;
    void print();

    // set functions
    void setIdentity();
    void setScale(float x, float y, float z);
    void setScale(float s);
    void setRotR(float angle, float x, float y, float z);
    void setRot(float angle, float x, float y, float z);
    void setTranslation(float x, float y, float z);

    // modify this matrix functions
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);
    void scale(float s);
    void translate(float x, float y, float z);
};

#endif //__MATRIX_H__
