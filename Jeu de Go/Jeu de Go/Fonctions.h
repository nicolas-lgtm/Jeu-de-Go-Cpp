#pragma once
#include "Header.h"

class Case;

void InitSDLElements(SDL_Window* window);
void InitMusic();
void InitUI();

void InitAll();

void Loop();

void CreateBackground(SDL_Renderer* a_renderer, SDL_Surface* a_bg, int a_x, int a_y, int a_sizeX, int a_sizeY);
void DisplayScoreBlanc();
void DisplayScoreNoir();

void CaptureEffects();

void KillAll();

float Round(float a_value);