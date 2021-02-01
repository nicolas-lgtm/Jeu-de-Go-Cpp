#pragma once
#include "Header.h"

class Case;

void DisplayCase(int x, int y);
void FreeCase(SDL_Surface* caseBMP);
void InitSprites();
void InitSDLElements(SDL_Window* window);
void Free(SDL_Window* window);