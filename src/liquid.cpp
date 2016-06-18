#include "liquid.hpp"

#include <memory.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <vector>

#include "./globals.h"
#include "./fpscounter.h"

Liquid liquid;

Particle::Particle(float x, float y, float vx, float vy, float density)
{
    this->x = x;
    this->y = y;

    this->vx = vx;
    this->vy = vy;

    this->density = density;

    pressure = 0.f;
    surface = 0.f;

    evx = vx;
    evy = vy;

    ax = 0.f;
    ay = 0.f;

    w = 0.f;
    h = 0.f;
}

void Particle::simulate(float gravx, float gravy)
{
    vx += ax / density + gravx;
    vy += ay / density + gravy;

    x += vx * deltatime;
    y += vy * deltatime;

    if(x >= width) {
        vx *= COLLISIONDAMP;
        x = width;
    } else if(x < 0) {
        vx *= COLLISIONDAMP;
        x = 0.f;
    }

    if(y >= height) {
        vy *= COLLISIONDAMP;
        y = height - 1;
    } else if(y < 0) {
        vy *= COLLISIONDAMP;
        y = 0.f;
    }

    evx = (evx + vx) / 2;
    evy = (evy + vy) / 2;
}

Liquid::Liquid()
{
    liquidbuffer_w = 0;
    liquidbuffer_h = 0;
    liquidbuffer = 0;

    kernel = 4.f;
    kernel2 = kernel * kernel;

    mass = 20.f;

    gravx = 0.f;
    gravy = 1.8f;

    gas = 0.0001f;
    viscosity = 0.64f;
    poly6 = 315.0f / (64.0f * PI * pow(kernel, 9));
    density = mass * poly6 * pow(kernel2, 3);

    spiky = -45.0f / (PI * pow(kernel, 6));
    visco = 45.0f / (PI * pow(kernel, 6));

    gradp6 = -945 / (32 * PI * pow(kernel, 9));
    lplcp6 = -945 / (8 * PI * pow(kernel, 9));

    surface = 0.06f;
    coe = 0.002f;

    baselplc = lplcp6 * mass * kernel2 * (0 - 3 / 4 * kernel2);

    quadtree = 0;
}

Liquid::~Liquid()
{
    for(auto p : particles) {
        delete p;
    }

    if(quadtree) {
        quadtree->clear();
        delete quadtree;
    }

    particles.clear();
    delete [] liquidbuffer;
}

void Liquid::init()
{
    if(liquidbuffer_w != width || liquidbuffer_h != height) {
        liquidbuffer_w = width;
        liquidbuffer_h = height;

        srand(time(0));

        liquidbuffer = new unsigned char[(liquidbuffer_w * liquidbuffer_h) * 4];

        for(auto p : particles) {
            delete p;
        }

        if(quadtree) {
            quadtree->clear();
            delete quadtree;
        }

        quadtree = new Quadtree<Particle>(0.f, 0.f, width, height, 5);
        particles.clear();

        // for(int x = 0; x < width; x+=3) {
        //     for(int y = 0; y < height; y+=3) {
        //         Particle *temp = new Particle(
        //             x,
        //             y,
        //             0.f,
        //             0.f,
        //             0.f
        //         );
        //         particles.push_back(temp);
        //         quadtree->addObject(temp);
        //     }
        // }
    }
}

void Liquid::plot(int x, int y, char r, char g, char b)
{
    if(x < liquidbuffer_w && y < liquidbuffer_h && x >= 0 && y >= 0) {
        unsigned int offset = (x + y * width) * 4;

        liquidbuffer[offset] = r;
        liquidbuffer[offset + 1] = g;
        liquidbuffer[offset + 2] = b;
    }
}

void Liquid::simulate()
{
    init();
    update();

    memset(liquidbuffer, 0, (width * height) * 4);

    static float time = 0.f;
    static int spawner = 0;
    static float vel = 0;
    time += deltatime;

    // if(time >= 30) {
    //     float old = gravx;
    //     gravx = gravy;
    //     gravy = -old;
    //     time = 0;
    // }

    if(time >= 0.001f) {
        spawner = (spawner == 0) ? width : 0;
        vel = ((rand() % 500) + 500) * 0.05f;
        vel = (spawner == 0) ? vel : -vel;

        if(particles.size() < MAXPARTICLES) {
            Particle *temp = new Particle(
                spawner,//rand() % width,
                10.f,//rand() % height,
                vel,
                0.f,
                0.f
            );

            particles.push_back(temp);
            quadtree->addObject(temp);
        }

        time = 0;
    }

    // std::vector<rect> qt = quadtree->visualize();
    //
    // for(auto r : qt) {
    //     for(int rx = 0; rx < r.w; rx++) {
    //         plot(r.x + rx, r.y + r.h, 0, 255, 0);
    //         plot(r.x + rx, r.y, 0, 255, 0);
    //     }
    //
    //     for(int ry = 0; ry < r.h; ry++) {
    //         plot(r.x + r.w, r.y + ry, 0, 255, 0);
    //         plot(r.x, r.y + ry, 0, 255, 0);
    //     }
    // }

    for(auto p : particles) {
        int color = 255 - (p->pressure * 255);
        if(color < 32) {
            color = 32;
        }

        plot(p->x, p->y, 0, color, 0);
    }

    quadtree->update();

    memcpy(pixels, liquidbuffer, (width * height) * 4);

    printFps();
}

void Liquid::update()
{
    for(auto p : particles) {
        p->ax = 0.f;
        p->ay = 0.f;

        p->density = 0.f;
        p->pressure = 0.f;

        float gradx = 0.f;
        float grady = 0.f;
        float lplc = 0.f;

        std::vector<Particle *> nearby = quadtree->getObjectsAt(p->x, p->y);

        for(auto np : nearby) {
            if(np == p) {
                continue;
            }

            float rx = np->x - p->x;
            float ry = np->y - p->y;
            float dist = rx * rx + ry * ry;

            if(dist < kernel2 && dist > 0) {
                p->density += mass * poly6 * pow(kernel2 - dist, 3);
            }
        }

        p->density += density;
        p->pressure = (pow(p->density, 7) - 1) * gas;

        for(auto np : nearby) {
            if(np == p) {
                continue;
            }

            float rx = p->x - np->x;
            float ry = p->y - np->y;
            float dist = rx * rx + ry * ry;

            if(dist < kernel2 && dist > 0) {
                float r = sqrt(dist);
                float V = mass / p->density;
                float rk = kernel - r;

                float sd = spiky * rk * rk;
                float force = V * (p->pressure + np->pressure) * sd;

                p->ax -= rx * force / r;
                p->ay -= ry * force / r;

                float rvx = np->evx - p->evx;
                float rvy = np->evy - p->evy;

                float vk = visco * rk;
                force = V * viscosity * vk;
                p->ax += rvx * force;
                p->ay += rvy * force;

                float temp = (-1) * gradp6 * V * pow(kernel2 - dist, 2);
                gradx += temp * rx;
                grady += temp * ry;

                lplc += lplcp6 * V * (kernel2 - dist) *
                        (dist - 3 / 4 * (kernel2 - dist));
            }
        }

        lplc += baselplc / p->density;
        p->surface = sqrt(gradx * gradx + grady * grady);

        // printf("%f\n", p->surface);

        if(p->surface > surface) {
            p->ax += coe * lplc * gradx / p->surface;
            p->ay += coe * lplc * grady / p->surface;
        }

        p->simulate(gravx, gravy);
    }
}
