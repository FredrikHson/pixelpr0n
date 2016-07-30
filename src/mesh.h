#ifndef __MESH_H__
#define __MESH_H__

#include "vec4.h"
#include "matrix.h"

void drawMesh(); // debug function

struct triangle
{
    vec4 pos[3];
    unsigned char color[3];
};
class mesh
{
private:
    unsigned int currentTris;
public:
    triangle* tris;
    unsigned int numTris;

    void draw(matrix& m);
    void setNumTris(unsigned int n);
    void addTris(triangle &tri);

    mesh();
    ~mesh();

};

#endif //__MESH_H__
