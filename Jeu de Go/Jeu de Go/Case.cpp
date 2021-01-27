#include "Header.h"
#include "Case.h"
#include "Fonctions.h"


Case::Case() { cout << "Case creee" << endl; }

Case::Case(int a_index, int a_tailleGoban) { 
	SetEtat(Etat::Vide);
	SetIndex(a_index, a_tailleGoban);

	int centerX = (SCREEN_WIDTH / 2) - (CASES_SIZE / 2 * a_tailleGoban);
	int centerY = (SCREEN_HEIGHT / 2) - (CASES_SIZE / 2 * a_tailleGoban);

	int x = centerX + GetIndex().first * CASES_SIZE;
	int y = centerY + GetIndex().second * CASES_SIZE;
	DisplayCase(x, y);
	cout << "Case x " + to_string(GetIndex().first) + " y " + to_string(GetIndex().second) + " creee" << endl;
}

Case::~Case() { 

	cout << "Case x : "  + to_string(coordonnees.first) + " y :" + to_string(coordonnees.second) + " detruite" << endl; 
}

void Case::SetIndex(int a_index, int a_tailleGoban) {
	coordonnees.first = a_index / a_tailleGoban;
	coordonnees.second = a_index % a_tailleGoban;
}

std::pair<int, int> Case::GetIndex() { return coordonnees; }

Etat Case::GetEtat() { return etat; }

void Case::SetEtat(Etat a_etat) { etat = a_etat; }

bool PeutPoser(bool a_noirJoue) { return true; }