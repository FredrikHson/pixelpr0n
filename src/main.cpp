#include <iostream>
#include <SDL.h>

int main(int argc, char** argv)
{
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2 Pixel Drawing",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                           SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    unsigned char* pixels = new unsigned char[640 * 480 * 3];

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


        for(unsigned int i = 0; i < 640 * 480 * 3; i++)
        {

            pixels[i] = rand() % 255;
        }

        //for(unsigned int y = 0; y < 480 - 1; y++)
        //{
            //for(unsigned int x = 0; x < 640; x++)
            //{
                //unsigned int thispixeloffset = x * 3 + y * 640 * 3;

                //pixels[thispixeloffset] = 0;
                //pixels[thispixeloffset+1] = 0;
                //pixels[thispixeloffset+2] = 0;
            //}
        //}


        SDL_UpdateTexture(texture, NULL, pixels, 640 * 3);
        SDL_RenderClear(renderer);
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
