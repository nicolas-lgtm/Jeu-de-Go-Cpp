#pragma once
#include "Header.h"
#include "Case.h"

using namespace std;
class Goban
{
protected:
	int taille;
	bool noirDoitJouer;
	int ptsNoir;
	int ptsBlanc;

	vector<vector<Case*>> cases;

	//vector<vector<Case*>> cases(taille, vector<Case> (taille));

public:
	Goban(int taille);
	Goban();
	~Goban();

	int GetTaille();
	void SetTaille(int taille);

	bool GetTourJoueur();
	void SetTourJoueur();

	int GetPtsNoir();
	void SetPtsNoir(int points_a_ajouter);	
	
	int GetPtsBlanc();
	void SetPtsBlanc(int points_a_ajouter);

};

