#pragma once
#include "Header.h"

class Case;
class Bouton;

int DetermineTailleGoban();

void InitSDLElements(SDL_Window* window);
void InitMusic();
void InitUI();
void InitGame(int a_tailleGoban);
void InitAll();

void LoopGame();

void CreateBackground(SDL_Renderer* a_renderer, SDL_Surface* a_bg, int a_x, int a_y, int a_sizeX, int a_sizeY);
void DisplayScoreBlanc(int a_sizeBackgroundX, int a_sizeBackgroundY);
void DisplayScoreNoir(int a_sizeBackgroundX, int a_sizeBackgroundY);

void CaptureEffects();
void  Rejouer();
void FinPartie();
void AfficherWinner(SDL_Surface* textParent, SDL_Surface* background);

void KillAll();

std::string Round(float a_value);

void ComptagePoints();