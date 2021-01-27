#pragma once
#include "Header.h"

class Case
{
protected:
	Etat etat;
	pair<int, int> coordonnees;
	bool peutJouer;

public:
	Case();
	Case(int a_index, int a_tailleGoban);

	~Case();

	void SetIndex(int a_index, int a_tailleGoban);
	pair<int, int> GetIndex();

	Etat GetEtat();
	void SetEtat(Etat a_etat);

	bool PeutPoser(bool a_noirJoue);
	void SetPeutPoser(bool a_peutPoser);
};



