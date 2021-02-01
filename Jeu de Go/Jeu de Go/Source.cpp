#include "Header.h"
#include "Case.h"
#include "Goban.h"

int tailleGoban = 19;

SDL_Renderer* renderer;

Goban* goban;

int main(int argc, char** argv)
{
    SDL_Window* window;

    bool quit = false;
    SDL_Event event;

    window = SDL_CreateWindow("Jeu de Go",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    InitSDLElements(window);

    goban = new Goban(renderer, window, tailleGoban);

    while (!quit)
    {
        SDL_WaitEvent(&event);

        SDL_Event e;

        while (!quit)
        {
            SDL_WaitEvent(&event);

            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            //case SDL_MOUSEBUTTONDOWN:
            //    printf("click");
            //    break;
            }

            for (int i = 0; i < tailleGoban; i++)
                for (int j = 0; j < tailleGoban; j++)
                    goban->cases[i][j]->handleEvent(&event);
        }
    }

    Free(window);
    SDL_Quit();

    return 0;
}



//Fonctions 





//void FreeCase(SDL_Surface* caseBMP) {
//    SDL_DestroyTexture(caseTexture);
//    SDL_FreeSurface(caseBMP);
//}

//void InitSprites() {
//    caseTexture = SDL_CreateTextureFromSurface(renderer, caseBMP);
//    if (!caseBMP) printf("Erreur de chargement de l'image : %s", SDL_GetError());
//}

void InitSDLElements(SDL_Window* window) {
    SDL_Init(SDL_INIT_VIDEO);
    renderer = SDL_CreateRenderer(window, -1, 0);
    //InitSprites();
}

void Free(SDL_Window* window) {
    //FreeCase(caseBMP);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}