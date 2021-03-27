#include <SDL.h>
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <stdio.h>

#include <iostream>

int WindowWidth = 1280;
int WindowHeight = 720;
SDL_Surface *WindowSurface;
SDL_Window *Window;
bool Running = true;

int main(int argc, char **argv)
{
SDL_Renderer *Renderer;
    SDL_Init(SDL_INIT_EVERYTHING);
    Window = SDL_CreateWindow("RaytRacer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    int P = SDL_RenderClear(Renderer);

     std::cout << "SDL_Init Error: " << P << SDL_GetError() << std::endl;

    //keys Keys;
    SDL_Event Event;
    while (Running)
    {
        SDL_PollEvent(&Event);
        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(Renderer);

        // SDL_RenderDrawLine(Renderer, 0, 0, WindowWidth, WindowHeight);

        int frameEnd = SDL_GetTicks();
        int frameTime = frameEnd - frameStart;
        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        // SDL_SetRenderDrawColor(Renderer, 25, 25, 25, 255);
        // SDL_RenderFillRect(Renderer, &R->PlayfieldRect);

        // SDL_SetRenderDrawColor(Renderer, 70, 70, 70, 255);
        // SDL_RenderClear(Renderer);
        SDL_RenderPresent(Renderer);
        //  std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    return 0;
}