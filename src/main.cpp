#include <SDL.h>
#include <unistd.h>
#include <memory.h>
#include "fire.h"
#include "sand.h"
#include "burningsand.h"
#include "liquid.hpp"
#include "globals.h"
#include "fpscounter.h"

unsigned int width    = 320;
unsigned int height   = 240;
unsigned char* pixels = 0;
float deltatime       = 1.0f;
float abstime         = 0.0f;

enum Effect
{
    FIRE,
    LIQUID,
    SAND,
    BURNING_SAND
};

void printhelp()
{
    printf("-f   fire effect\n");
    printf("-l   liquid effect\n");
    printf("-s   sand effect\n");
}
int main(int argc, char** argv)
{
    bool quit = false;

    // add more effects as bools here and then in getopt+printhelp+in the while loop
    Effect effect;

    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
                             "pixelpr0n",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             width * 2,
                             height * 2,
                             SDL_WINDOW_RESIZABLE
                         );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(
                               renderer,
                               SDL_PIXELFORMAT_ABGR8888,
                               SDL_TEXTUREACCESS_STATIC,
                               width,
                               height
                           );

    pixels = new unsigned char[width * height * 4];
    memset(pixels, 0, width * height * 4);
    long last = 0;
    int c;

    if(argc != 2)
    {
        printhelp();
        return 0;
    }

    while((c = getopt(argc, argv, "hflsb")) != -1)
    {
        switch(c)
        {

            case 'f':
            {
                effect = FIRE;
                break;
            }

            case 'l':
            {
                effect = LIQUID;
                liquid.init();
                break;
            }

            case 's':
            {
                effect = SAND;
                break;
            }

            case 'b':
            {
                effect = BURNING_SAND;
                break;
            }

            case 'h':
            default:
            {
                printhelp();
                return 0;
            }
        }
    }

    while(!quit)
    {

        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_WINDOWEVENT:
                {
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            if(pixels)
                            {
                                delete [] pixels;
                            }

                            width = event.window.data1 / 2;
                            height = event.window.data2 / 2;
                            pixels = new unsigned char[width * height * 4];
                            memset(pixels, 0, width * height * 4);

                            if(texture)
                            {
                                SDL_DestroyTexture(texture);
                            }

                            texture = SDL_CreateTexture(
                                          renderer,
                                          SDL_PIXELFORMAT_ABGR8888,
                                          SDL_TEXTUREACCESS_STATIC,
                                          width,
                                          height
                                      );
                            break;
                    }

                    break;
                }
            }
        }

        const unsigned char* state = SDL_GetKeyboardState(0);

        if(state[SDL_SCANCODE_ESCAPE])
        {
            quit = true;
        }

        long now = SDL_GetTicks();
        deltatime = ((float)(now - last)) / 1000.0f;;
        last = now;
        abstime += deltatime;

        switch(effect)
        {
            case FIRE:
            {
                drawFire();
                break;
            }

            case LIQUID:
            {
                liquid.simulate();
                break;
            }

            case SAND:
            {
                drawSand();
                break;
            }

            case BURNING_SAND:
            {
                drawBurningSand();
                break;
            }
        }

        SDL_UpdateTexture(texture, NULL, pixels, width * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    switch(effect)
    {
        case FIRE:
            destroyFire();
            break;

        case SAND:
            destroySand();
            break;

        case BURNING_SAND:
            destroyBurningSand();

        default:
            break;
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
