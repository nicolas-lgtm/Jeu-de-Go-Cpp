#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

#include <time.h>
#include <vector>

#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <vector>

#include "Fonctions.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int CASES_SIZE = 25;


enum class Etat
{
	Vide = 0,
	Noir = 1,
	Blanc = 2
};
