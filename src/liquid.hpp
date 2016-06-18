#ifndef LIQUID_HPP_
#define LIQUID_HPP_

#define COLLISIONDAMP -0.0f
#define MAXPARTICLES 2000

#define PI 3.14

#include <inttypes.h>
#include <math.h>

#include <vector>

#include "./quadtree.hpp"

class Particle
{
public:
    Particle(float x, float y, float vx, float vy, float density);
    virtual ~Particle() {}
    void simulate(float gravx, float gravy);

    float x;
    float y;
    float h;
    float w;

    float vx;
    float vy;

    float evx;
    float evy;

    float ax;
    float ay;

    float density;
    float pressure;
    float surface;
};

class Liquid
{
public:
    Liquid();
    virtual ~Liquid();

    void init();
    void simulate();
private:
    void update();
    void plot(int x, int y, char r, char g, char b);

    int liquidbuffer_w;
    int liquidbuffer_h;
    unsigned char *liquidbuffer;

    float kernel;
    float kernel2;
    float mass;
    float spiky;
    float viscosity;
    float visco;
    float poly6;
    float gas;
    float density;

    float gravx;
    float gravy;

    float gradp6;
    float lplcp6;
    float surface;
    float coe;
    float baselplc;

    Quadtree<Particle> *quadtree;
    std::vector<Particle *> particles;
};

extern Liquid liquid;

#endif // LIQUID_HPP_
