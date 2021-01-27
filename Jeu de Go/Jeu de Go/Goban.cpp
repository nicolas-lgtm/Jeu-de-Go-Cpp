#include "Header.h"
#include "Goban.h"
#include "Case.h"
#include "Fonctions.h"

Goban::Goban() {
	int compteur = 0;

	for (int i = 0; i < 19; i++) {
		vector<Case*> ligne;
		cases.push_back(ligne);

		for (int j = 0; j < 19; j++) {
			Case* _case = new Case(compteur, 19);
			cases[i].push_back(_case);
			compteur++;
		}
	}
}

Goban::Goban(int tailleGoban) {
	int index = 0;

	for (int i = 0; i < tailleGoban; i++) {
		vector<Case*> ligne;
		cases.push_back(ligne);

		for (int j = 0; j < tailleGoban; j++) {
			Case* _case = new Case(index, tailleGoban);
			cases[i].push_back(_case);
			index++;
		}
	}		
}

Goban::~Goban() {
	cout << "Destruction du Goban" << endl;
}


int Goban::GetTaille() {
	return taille;
}

void Goban::SetTaille(int a_taille) {
	taille = a_taille;
}

bool Goban::GetTourJoueur() {
	return noirDoitJouer;
}

void Goban::SetTourJoueur() {
	noirDoitJouer = !noirDoitJouer;
}

int Goban::GetPtsNoir() {
	return ptsNoir;
}

void Goban::SetPtsNoir(int a_points_a_ajouter) {
	ptsNoir += a_points_a_ajouter;
}

int Goban::GetPtsBlanc() {
	return taille;
}

void Goban::SetPtsBlanc(int a_points_a_ajouter) {
	ptsBlanc += a_points_a_ajouter;
}