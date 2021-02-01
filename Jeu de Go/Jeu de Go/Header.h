#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <string>

#include <time.h>
#include <vector>
#include <SDL.h>
#include <SDL_surface.h>

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <vector>

#include "Fonctions.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int CASES_SIZE = 35;


enum class Etat
{
	Vide = 0,
	Noir = 1,
	Blanc = 2
};
