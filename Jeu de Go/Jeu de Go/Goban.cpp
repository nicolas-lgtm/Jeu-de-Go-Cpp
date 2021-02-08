#include "Header.h"
#include "Goban.h"
#include "Case.h"
#include "Fonctions.h"

class Case;

Goban::Goban(SDL_Renderer* renderer, SDL_Window* window, int tailleGoban) {
	int index = 0;

	SDL_Surface* vide = SDL_LoadBMP("Images\\case.bmp");
	SDL_Surface* noir = SDL_LoadBMP("Images\\noir.bmp");
	SDL_Surface* blanc = SDL_LoadBMP("Images\\blanc.bmp");


	SDL_Texture* videTexture = SDL_CreateTextureFromSurface(renderer, vide);
	SDL_Texture* noirTexture = SDL_CreateTextureFromSurface(renderer, noir);
	SDL_Texture* blancTexture = SDL_CreateTextureFromSurface(renderer, blanc);

	for (int i = 0; i < tailleGoban; i++) {
		vector<Case*> ligne;
		cases.push_back(ligne);

		for (int j = 0; j < tailleGoban; j++) {
			Case* newCase = new Case(this, renderer, window, videTexture, index, tailleGoban);
			cases[i].push_back(newCase);
			index++;
		}
	}		

	SetTaille(tailleGoban);
}

Goban::~Goban() { cout << "Destruction du Goban" << endl; }

int Goban::GetTaille() { return taille; }
void Goban::SetTaille(int a_taille) { taille = a_taille; }

bool Goban::GetCapture() { return capture; }
void Goban::SetCapture(bool a_capture) { capture = a_capture; }

bool Goban::GetNoirDoitJouer() { return noirDoitJouer; }
void Goban::SetTourJoueur() { noirDoitJouer = !noirDoitJouer; }

Etat Goban::GetTypeJoueur() {
	if (GetNoirDoitJouer())       return Etat::Noir;
	else if(!GetNoirDoitJouer())  return Etat::Blanc;
}

int Goban::GetPtsNoir() { return ptsNoir; }
void Goban::AjoutPointNoir() { ptsNoir++; }

float Goban::GetPtsBlanc() { return ptsBlanc; }
void Goban::AjoutPointBlanc() { ptsBlanc++; }

void Goban::ResetHasBeenCheckedParameter() {
	for (int i = 0; i < GetTaille(); i++)
		for (int j = 0; j < GetTaille(); j++)
			if (cases[i][j]->GetHasBeenChecked()) cases[i][j]->SetHasBeenChecked(false);
}

void Goban::SetCaseIndispo(Case* a_case) { 
	caseIndispo = a_case;
}

Case* Goban::GetCaseIndispo() {
	return caseIndispo;
}

void Goban::SetTourIndispo(bool a_tourIndispo) {
	tourIndispo = a_tourIndispo;
}

bool Goban::GetTourIndispo() {
	return tourIndispo;
}

void Goban::ReinitCaseIndispo() { SetCaseIndispo(NULL); }

void Goban::Passer() {
	SetTourJoueur();

	if (GetTourPasse()) {
		FinPartie();
	}
	else {
		SetTourPasse(true);
	}
}

void Goban::SetTourPasse(bool a_tourPasse) { tourPasse = a_tourPasse; }
bool Goban::GetTourPasse() { return tourPasse; }
void Goban::Abandonner() { FinPartie(); }
void Goban::FinPartie() { cout << "Partie fini !" << endl; }
