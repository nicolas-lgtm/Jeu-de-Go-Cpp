#pragma once
#include "Header.h"
class Case;

using namespace std;
class Goban
{
protected:
	int taille;
	bool noirDoitJouer;
	int ptsNoir;
	int ptsBlanc;
	SDL_Renderer* p;
	//vector<vector<Case*>> cases(taille, vector<Case> (taille));
public:
	vector<vector<Case*>> cases;

	Goban(SDL_Renderer* renderer, SDL_Window* window, int taille);
	~Goban();

	int GetTaille();
	void SetTaille(int taille);

	bool GetNoirDoitJouer();
	void SetTourJoueur();

	int GetPtsNoir();
	void SetPtsNoir(int points_a_ajouter);	
	
	int GetPtsBlanc();
	void SetPtsBlanc(int points_a_ajouter);

	void ResetHasBeenCheckedParameter();
};

