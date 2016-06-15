#include <iostream>
#include <SDL.h>

int main(int argc, char** argv)
{
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("pixelpr0n",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                           SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 320, 240);
    unsigned char* pixels = new unsigned char[320 * 240 * 4];

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


        for(unsigned int i = 0; i < 320*240; i++)
        {

            pixels[i*4] = rand() % 255;
            pixels[i*4+1] = rand() % 255;
            pixels[i*4+2] = rand() % 255;
        }

        for(unsigned int y = 1; y < rand()%240 ; y++)
        {
            for(unsigned int x = 0; x < 320; x++)
            {
                unsigned int thispixeloffset = x * 4 + y * 320 * 4;
                unsigned int underpixeloffset = x * 4 + (y-1) * 320 * 4;

                pixels[thispixeloffset] = pixels[underpixeloffset];
                pixels[thispixeloffset + 1] = pixels[underpixeloffset+1];
                pixels[thispixeloffset + 2] = pixels[underpixeloffset+2];
            }
        }


        SDL_UpdateTexture(texture, NULL, pixels, 320 * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
