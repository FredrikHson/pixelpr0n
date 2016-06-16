#include <SDL.h>
#include <unistd.h>
#include "fire.h"
#include "globals.h"

unsigned int width = 320;
unsigned int height = 240;
unsigned char* pixels = 0;
float deltatime = 1.0f;
void printhelp()
{
    printf("-f   fire effect\n");
}
int main(int argc, char** argv)
{
    bool quit = false;

    // add more effects as bools here and then in getopt+printhelp+in the while loop
    bool fire = false;

    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("pixelpr0n",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                           SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 320, 240);
    pixels = new unsigned char[320 * 240 * 4];
    long last = 0;
    int c;

    if(argc != 2)
    {
        printhelp();
        return 0;
    }

    while((c = getopt(argc, argv, "hf")) != -1)
    {
        switch(c)
        {

            case 'f':
            {
                fire = true;
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
            }
        }

        long now = SDL_GetTicks();
        deltatime = ((float)(now - last)) / 1000.0f;;
        last = now;

        if(fire)
        {
            drawFire();
        }

        SDL_UpdateTexture(texture, NULL, pixels, 320 * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    destroyFire();
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
