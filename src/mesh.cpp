#include "matrix.h"
#include "vec4.h"
#include "mesh.h"
#include "drawshapes.h"
#include "globals.h"
#include <memory.h>
#include "fpscounter.h"
#include <math.h>
#include "fast_obj_loader.h"
#include <stdio.h>

void mesh::draw(matrix& m)
{
    static float minz = 100000;
    static float maxz = -100000;

    for(unsigned int i = 0; i < numTris; i++)
    {
        vec4 tpos[3];
        tpos[0] = m * tris[i].pos[0];
        tpos[1] = m * tris[i].pos[1];
        tpos[2] = m * tris[i].pos[2];
        tpos[0].divw();
        tpos[1].divw();
        tpos[2].divw();


        for(unsigned int j = 0; j < 3; j++)
        {
            if(tpos[j][2] < minz)
            {
                minz = tpos[j][2];
            }

            if(tpos[j][2] > maxz)
            {
                maxz = tpos[j][2];
            }

            unsigned int j2    = (j + 1) % 3;
            drawLine(tpos[j][0], tpos[j][1], tpos[j][2],
                     tpos[j2][0], tpos[j2][1], tpos[j][2],
                     tris[i].color[0], tris[i].color[1], tris[i].color[2]);

        }
    }

    //printf("%f,%f\n", minz, maxz);
}
void mesh::setNumTris(unsigned int n)
{
    if(n == 0)
    {
        return;
    }

    if(n == numTris)
    {
        return;
    }

    if(tris)
    {
        delete [] tris;
    }

    numTris     = n;
    tris        = new tri[numTris];
    currentTris = 0;
}

void mesh::addTris(tri& tri)
{
    if(currentTris >= numTris)
    {
        return;
    }

    tris[currentTris] = tri;
    currentTris++;
}
mesh::mesh()
{
    tris        = 0;
    numTris     = 0;
    currentTris = 0;
}

mesh::~mesh()
{
    if(tris)
    {
        delete [] tris;
    }
}




void drawMesh()
{
    static bool inited = false;
    static mesh cube;
    static mesh suzanne;

    if(!inited)
    {
        suzanne.loadFromObj("./data/suzanne.obj");
    }

    matrix model;
    model.setIdentity();
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0, -20);

    //model.scale();
    matrix final =  model * perspective * view ;
    memset(pixels, 0, width * height * 4);
    for(int i=0;i<width*height;i++)
    {
        zbuffer[i]=1000;
    }
    suzanne.draw(final);

    model.setIdentity();
    model.translate(3, 0 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    model.setIdentity();
    model.translate(-3, 0 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    model.setIdentity();
    model.translate(0, 3 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    model.setIdentity();
    model.translate(0, -3 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    model.setIdentity();
    model.translate(0, 0 , 3);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    model.setIdentity();
    model.translate(0, 0 , -3);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -20);
    final =  model * perspective * view ;
    suzanne.draw(final);

    printFps();

}
void mesh::loadFromObj(const char* filename)
{

    obj* objfile = loadObj(filename);

    if(objfile)
    {
        setNumTris(objfile->numfaces - 1);

        for(int i = 0; i < objfile->numfaces; i++)
        {
            tri currenttri;//={vec4(0, 0, 0), vec4(0, 0, 0), vec4(0, 0, 0), {0, 0, 0}};
            currenttri.pos[0][0] = objfile->verts[objfile->faces[i].verts[0]].x;
            currenttri.pos[0][1] = objfile->verts[objfile->faces[i].verts[0]].y;
            currenttri.pos[0][2] = objfile->verts[objfile->faces[i].verts[0]].z;
            currenttri.pos[0][3] = 1;
            currenttri.pos[1][0] = objfile->verts[objfile->faces[i].verts[1]].x;
            currenttri.pos[1][1] = objfile->verts[objfile->faces[i].verts[1]].y;
            currenttri.pos[1][2] = objfile->verts[objfile->faces[i].verts[1]].z;
            currenttri.pos[1][3] = 1;
            currenttri.pos[2][0] = objfile->verts[objfile->faces[i].verts[2]].x;
            currenttri.pos[2][1] = objfile->verts[objfile->faces[i].verts[2]].y;
            currenttri.pos[2][2] = objfile->verts[objfile->faces[i].verts[2]].z;
            currenttri.pos[2][3] = 1;
            currenttri.color[0] = (objfile->normals[objfile->faces[i].normals[0]].x + 1) * 127;
            currenttri.color[1] = (objfile->normals[objfile->faces[i].normals[0]].y + 1) * 127;
            currenttri.color[2] = (objfile->normals[objfile->faces[i].normals[0]].z + 1) * 127;
            //printf("%f,%f,%f\n",currenttri.pos[0][0],currenttri.pos[0][1],currenttri.pos[0][2]);
            addTris(currenttri);
        }
    }
    else
    {
        numTris = 0;
    }
}
