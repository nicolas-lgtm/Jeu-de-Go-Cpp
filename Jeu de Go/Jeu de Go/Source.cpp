//Using SDL and standard IO
#include "Header.h"
#include "Case.h"
#include "Goban.h"
/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/


Goban* goban;
int tailleGoban = 9;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* texture;

void DisplayCase(int x, int y) {
    printf("Display case");

    SDL_Surface* caseBMP = SDL_LoadBMP("Images\\case.bmp");

    if (!caseBMP) printf("Erreur de chargement de l'image : %s", SDL_GetError());
    texture = SDL_CreateTextureFromSurface(renderer, caseBMP);

    SDL_Rect dstrect = { x, y, CASES_SIZE, CASES_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_RenderPresent(renderer);
}



void FreeCase(SDL_Surface* caseBMP) {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(caseBMP);
}


int main(int argc, char** argv)
{
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Jeu de Go",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    goban = new Goban(19);

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}