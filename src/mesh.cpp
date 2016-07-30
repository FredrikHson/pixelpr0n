#include "matrix.h"
#include "vec4.h"
#include "mesh.h"
#include "drawshapes.h"
#include "globals.h"
#include <memory.h>
#include "fpscounter.h"
#include <math.h>

void mesh::draw(matrix& m)
{
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
            unsigned int j2    = (j + 1) % 3;
            drawLine(tpos[j][0], tpos[j][1],
                     tpos[j2][0], tpos[j2][1],
                     tris[i].color[0], tris[i].color[1], tris[i].color[2]);

        }
    }
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
    tris        = new triangle[numTris];
    currentTris = 0;
}

void mesh::addTris(triangle& tri)
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

    if(!inited)
    {
        triangle cube_data[] =
        {

            vec4(-1, 1, -1), vec4(-1, -1, 1), vec4(-1, 1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, -1), vec4(-1, -1, -1), vec4(-1, 1, -1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, 1), vec4(1, -1, -1), vec4(1, 1, -1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(-1, 1, 1), vec4(1, -1, 1), vec4(1, 1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(-1, -1, -1), vec4(1, -1, 1), vec4(-1, -1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, -1), vec4(-1, 1, 1), vec4(1, 1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(-1, 1, -1), vec4(-1, -1, -1), vec4(-1, -1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, -1), vec4(1, -1, -1), vec4(-1, -1, -1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, 1), vec4(1, -1, 1), vec4(1, -1, -1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(-1, 1, 1), vec4(-1, -1, 1), vec4(1, -1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(-1, -1, -1), vec4(1, -1, -1), vec4(1, -1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
            vec4(1, 1, -1), vec4(-1, 1, -1), vec4(-1, 1, 1), {(unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255)},
        };
        cube.setNumTris(12);

        for(int i = 0; i < 12; i++)
        {
            cube.addTris(cube_data[i]);
        }

    }

    matrix model;
    model.setIdentity();
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0, -10);

    //model.scale();
    matrix final =  model * perspective * view ;
    memset(pixels, 0, width * height * 4);
    cube.draw(final);

    model.setIdentity();
    model.translate(3, 0 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    model.setIdentity();
    model.translate(-3, 0 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    model.setIdentity();
    model.translate(0, 3 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    model.setIdentity();
    model.translate(0, -3 , 0);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    model.setIdentity();
    model.translate(0, 0 , 3);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    model.setIdentity();
    model.translate(0, 0 , -3);
    model.rotate(abstime * 25, 0, 1, 0);
    model.rotate(abstime * 25, 1, 0, 0);
    model.translate(0, 0 , -10);
    final =  model * perspective * view ;
    cube.draw(final);

    printFps();

}
