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
			//sprite = milieu;
			//if (i == 0) {//Bord gauche
			//	sprite = gauche;
			//	
			//	if (j == 0)	sprite = hautGauche;//Coin haut gauche
			//	else if (j == tailleGoban - 1) sprite = basGauche; //Coin bas gauche
			//}
			//else if (i == tailleGoban - 1) {//Bord droit
			//	sprite = droite;
			//	if (j == 0)	sprite = hautDroite;//Coin haut gauche
			//	else if (j == tailleGoban - 1) sprite = basDroite; //Coin bas gauche
			//} else if (j == 0) {
			//	sprite = haut;
			//}
			//else if (j == tailleGoban - 1) {
			//	sprite = bas;
			//}
			//if (!sprite) printf("Erreur de chargement de l'image : %s", SDL_GetError());
			//caseTexture = SDL_CreateTextureFromSurface(renderer, sprite);

			Case* newCase = new Case(this, renderer, window, videTexture, index, tailleGoban);
			cases[i].push_back(newCase);
			index++;
		}
	}		

	SetTaille(tailleGoban);
}

Goban::~Goban() {
	cout << "Destruction du Goban" << endl;
}

int Goban::GetTaille() { return taille; }
void Goban::SetTaille(int a_taille) { taille = a_taille; }

bool Goban::GetNoirDoitJouer() { return noirDoitJouer; }
void Goban::SetTourJoueur() { noirDoitJouer = !noirDoitJouer; }

int Goban::GetPtsNoir() { return ptsNoir; }
void Goban::SetPtsNoir(int a_points_a_ajouter) { ptsNoir += a_points_a_ajouter; }

int Goban::GetPtsBlanc() { return taille; }
void Goban::SetPtsBlanc(int a_points_a_ajouter) { ptsBlanc += a_points_a_ajouter; }

void Goban::ResetHasBeenCheckedParameter() {
	for (int i = 0; i < GetTaille(); i++)
		for (int j = 0; j < GetTaille(); j++)
			if (cases[i][j]->GetHasBeenChecked()) cases[i][j]->SetHasBeenChecked(false);

}