//Si on veut utiliser les méthodes d'une classe il faut inclure son header. Sinon on peut juste inclure 'class nomDeLaClasse;'.

#include "Case.h"
#include "Fonctions.h"
#include "Goban.h"
#include "Groupe.h"

class Goban;

SDL_Surface* caseVideSurface = SDL_LoadBMP("Images\\case.bmp");
SDL_Surface* caseNoireSurface = SDL_LoadBMP("Images\\noir.bmp");
SDL_Surface* caseBlancheSurface = SDL_LoadBMP("Images\\blanc.bmp");

//Constructeurs
Case::Case() { cout << "Case creee" << endl; }

Case::Case(Goban* goban, SDL_Renderer* renderer, SDL_Window* a_window, SDL_Texture* caseTexture, int a_index, int a_tailleGoban) {
	SetEtat(Etat::Vide);
	SetIndex(a_index, a_tailleGoban);

	p_goban = goban;

	int centerX = (SCREEN_WIDTH / 2) - (CASES_SIZE / 2 * a_tailleGoban);
	int centerY = (SCREEN_HEIGHT / 2) - (CASES_SIZE / 2 * a_tailleGoban);

	int x = centerX + GetIndex().first * CASES_SIZE;
	int y = centerY + GetIndex().second * CASES_SIZE;

	p_renderer = renderer;
	p_window = a_window;

	SetPositionInPx(x, y);
	SetTexture(caseTexture);
}

//Destructeur
Case::~Case() { cout << "Case x : " + to_string(coordonnees.first) + " y :" + to_string(coordonnees.second) + " detruite" << endl; }

//Gérer le click sur les cases
void Case::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		int thisCaseX = GetPositionInPx().first;
		int thisCaseY = GetPositionInPx().second;

		//Check if mouse is in button
		bool inside = true;

		if		(x <= thisCaseX)			 inside = false;
		else if (x > thisCaseX + CASES_SIZE) inside = false;
		else if (y <= thisCaseY)			 inside = false;
		else if (y > thisCaseY + CASES_SIZE) inside = false;

		if (inside) {
			system("cls");

			if (!CaseEstVide()) return;

			etat = p_goban->GetTypeJoueur();

			p_goban->ResetHasBeenCheckedParameter();
			this->SetHasBeenChecked(true);

			vector<Case*> groupe = GroupeDePierres(this);
			p_goban->ResetHasBeenCheckedParameter();

			if (!SeSuicide(groupe)) CreerCase();
			else etat = Etat::Vide;
		}
	}
}

void Case::CreerCase() {
	p_goban->SetTourJoueur();

	SDL_Surface* newSurface = caseVideSurface;


	switch (etat) {
	case Etat::Noir:
		newSurface = caseNoireSurface;
		break;
	case Etat::Blanc:
		newSurface = caseBlancheSurface;
		break;
	}

	SDL_DestroyTexture(GetTexture());

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(p_renderer, newSurface);
	SetTexture(newTexture);
}

//Index de la case
void Case::SetIndex(int a_index, int a_tailleGoban) {
	coordonnees.first = a_index / a_tailleGoban;
	coordonnees.second = a_index % a_tailleGoban;
}

std::pair<int, int> Case::GetIndex() { return coordonnees; }

//Position de la case en pixels
void Case::SetPositionInPx(int x, int y) {
	positionInPx.first = x;
	positionInPx.second = y;
}

pair<int, int> Case::GetPositionInPx() { return positionInPx; }


//Etat de la case : vide, noire ou blanche
Etat Case::GetEtat() { return etat; }


Etat Case::GetEtatContraire(Etat a_etat) {
	if (a_etat == Etat::Noir) return Etat::Blanc;
	if (a_etat == Etat::Blanc) return Etat::Noir;
}


void Case::SetEtat(Etat a_etat) { etat = a_etat; }

bool Case::CaseEstVide() { return etat == Etat::Vide; }

bool Case::SeSuicide(vector<Case*> a_groupe) { //Si est entourée exclusivement de cases ennemies SAUF s'il va tuer

	vector <vector<Case*>> tousGroupesEnnemis; //Vecteur qui contient sur chaque case, un groupe de cases ennemies
	vector <Case*> tempVector;

	for (int i = 0; i < a_groupe.size(); i++) {

		vector <Case*> entourage = GetCasesAutour(a_groupe[i]);

		for (int j = 0; j < entourage.size(); j++)
		{
			if (entourage[j]->GetEtat() == GetEtatContraire(p_goban->GetTypeJoueur()) && (!entourage[j]->GetHasBeenChecked())) {
				tempVector = GroupeDePierres(entourage[j]);

				if (!tempVector.empty()) {
					tousGroupesEnnemis.push_back(tempVector);
					tempVector.clear();
				}
			}
		}
	}

	for (int i = 0; i < tousGroupesEnnemis.size(); i++)
	{
		for (int j = 0; j < tousGroupesEnnemis[i].size(); j++)
		{
			if (GroupeEstCapture(tousGroupesEnnemis[i])) { 	//Si un groupe ennemis n'a pas de libertés, alors ça veut dire qu'on le tue donc on peut poser sa pierre
				for (int j = 0; j < tousGroupesEnnemis[i].size(); j++)
					tousGroupesEnnemis[i][j]->Effacer();
			}
		}
	}

	if (GroupeEstCapture(a_groupe)) return true;

	return false;
}


vector<Case*> Case::GetCasesAutour(Case* a_case) {
	vector<Case*> casesAutour;

	int x = a_case->GetIndex().first;
	int y = a_case->GetIndex().second;

	int tailleGoban = p_goban->GetTaille();

	if (x - 1 >= 0)			 casesAutour.push_back(p_goban->cases[x - 1][y]);
	if (x + 1 < tailleGoban) casesAutour.push_back(p_goban->cases[x + 1][y]);
	if (y - 1 >= 0)			 casesAutour.push_back(p_goban->cases[x][y - 1]);
	if (y + 1 < tailleGoban) casesAutour.push_back(p_goban->cases[x][y + 1]);

	return casesAutour;
}


bool Case::HasLibertes() {
	vector<Case*> casesAutour = GetCasesAutour(this);

	for (int i = 0; i < casesAutour.size(); i++)
		if (casesAutour[i]->etat == Etat::Vide) return true;

	return false;
}

//Si on a déjà fait des vérifications sur cette case
void Case::SetHasBeenChecked(bool a_hasBeenCheckedForGroupe) { hasBeenChecked = a_hasBeenCheckedForGroupe; }
bool Case::GetHasBeenChecked() { return hasBeenChecked; }


//Image qui représente la case
void Case::SetTexture(SDL_Texture* a_caseTexture) {
	int x = GetPositionInPx().first;
	int y = GetPositionInPx().second;

	SDL_Rect dstrect = { x, y, CASES_SIZE, CASES_SIZE };

	caseTexture = a_caseTexture;

	SDL_RenderCopy(p_renderer, caseTexture, NULL, &dstrect);
	SDL_RenderPresent(p_renderer);
}


SDL_Texture* Case::GetTexture() { return caseTexture; }


void Case::Effacer() {
	SetEtat(Etat::Vide);

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(p_renderer, caseVideSurface);
	SetTexture(newTexture);
}

void Case::SetIndispo(bool a_indispo) { indisponibleUnTour = a_indispo; }

bool Case::GetIndispo() { return indisponibleUnTour; }



//Fonctions hors de la classe :

bool GroupeEstCapture(vector<Case*> a_groupe) {
	for (int i = 0; i < a_groupe.size(); i++)
		if (a_groupe[i]->HasLibertes()) return false;

	return true;
}


//Constitue le groupe liée à la pierre envoyée en param_tre
vector<Case*> GroupeDePierres(Case* a_case) {
	//cout << idRecursivite << endl;
	vector<Case*> tempVector; //Stocke pierres alliées qui n'ont pas été checkées
	vector<Case*> casesAutour = a_case->GetCasesAutour(a_case);//Récupère les cases autour de la pierre passée en paramètre

	if (tempVector.empty()) tempVector.push_back(a_case); //Si le vecteur est vide, ça signifique que la pierre est isolée. Donc elle est seule dans son groupe. Il faut l'ajouter manuellement.

	for (int i = 0; i < casesAutour.size(); i++) //On parcourt les cases autour de la case passée en paramètre
	{
		Case* newCase = casesAutour[i];

		if (newCase->GetEtat() == a_case->GetEtat() && newCase->GetHasBeenChecked() == false) { //Si la case est alliée de cette passée en paramètre ET qu'elle n'a pas été checkée...
			newCase->SetHasBeenChecked(true);													//...on indique qu'elle a été checkée...
			vector <Case*> tempVector2 = GroupeDePierres(newCase);				//...on constitue depuis cette pierre...
			tempVector.insert(end(tempVector), begin(tempVector2), end(tempVector2));			//...on ajoute le groupe de cette pierre au groupe de pierre déjà existant.
		}
	}


	return tempVector; //Retourne le groupe constitué
}

//Fonctions de debug :

void AfficherVecteur(vector<Case*> a_vecteur) {
	for (int i = 0; i < a_vecteur.size(); i++) {
		AfficherCoordonnees(a_vecteur[i]);
	}

	cout << endl;
}
void AfficherCoordonnees(Case* _case) { cout << "x : " << _case->GetIndex().first << " y : " << _case->GetIndex().second << " " << endl; }
void AfficherEtat(Etat a_etat) {
	if (a_etat == Etat::Vide) {
		cout << "Vide ";
	}
	else if (a_etat == Etat::Noir) {
		cout << "Noir ";
	}
	else if (a_etat == Etat::Blanc) {
		cout << "Blanc ";
	}
}