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

	Case* caseIndispo;
	bool tourIndispo = false;
	//vector<vector<Case*>> cases(taille, vector<Case> (taille));
public:
	vector<vector<Case*>> cases;

	Goban(SDL_Renderer* renderer, SDL_Window* window, int taille);
	~Goban();

	int GetTaille();
	void SetTaille(int taille);

	bool GetNoirDoitJouer();
	void SetTourJoueur();
	Etat GetTypeJoueur();

	int GetPtsNoir();
	void SetPtsNoir(int points_a_ajouter);	
	
	int GetPtsBlanc();
	void SetPtsBlanc(int points_a_ajouter);

	void ResetHasBeenCheckedParameter();
	void ResetHasBeenCheckedByAlliesParameter();

	Case* GetCaseIndispo();
	void SetCaseIndispo(Case* a_case);

	void SetTourIndispo(bool a_tourIndispo);
	bool GetTourIndispo();

	void ReinitCaseIndispo();
};

