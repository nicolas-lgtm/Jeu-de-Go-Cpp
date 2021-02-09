#pragma once
#include "Header.h"
class Case;

using namespace std;
class Goban
{
protected:
	int taille;
	bool noirDoitJouer = true;
	int ptsNoir = 0;
	float ptsBlanc = 6.5f;

	Case* caseIndispo;
	bool tourIndispo = false;
	bool tourPasse = false;
	bool capture = false;

public:
	vector<vector<Case*>> cases;

	Goban(SDL_Renderer* renderer, SDL_Window* window, int taille);
	~Goban();

	int GetTaille();
	void SetTaille(int taille);

	void SetCapture(bool a_capture);
	bool GetCapture();

	bool GetNoirDoitJouer();
	void SetTourJoueur();
	Etat GetTypeJoueur();

	int GetPtsNoir();
	void AjoutPointNoir();	
	
	float GetPtsBlanc();
	void AjoutPointBlanc();

	void ResetHasBeenCheckedParameter();

	Case* GetCaseIndispo();
	void SetCaseIndispo(Case* a_case);

	void SetTourIndispo(bool a_tourIndispo);
	bool GetTourIndispo();

	void ReinitCaseIndispo();

	void Passer();
	void SetTourPasse(bool a_tourPasse);
	bool GetTourPasse();
	void Abandonner();
};

