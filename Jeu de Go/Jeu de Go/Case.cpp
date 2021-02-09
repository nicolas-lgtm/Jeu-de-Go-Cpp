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

			bool doitResetCaseIndispo = false;

			if (!CaseEstVide() || this == p_goban->GetCaseIndispo()) {
				cout << "Case occupee" << endl;
				return;
			}

			if (p_goban->GetTourPasse()) p_goban->SetTourPasse(false);

			if (p_goban->GetCaseIndispo()) doitResetCaseIndispo = true;

			etat = p_goban->GetTypeJoueur();

			p_goban->ResetHasBeenCheckedParameter();

			vector<Case*> groupe = GroupeDeCases(this);
			p_goban->ResetHasBeenCheckedParameter();

			if (!SeSuicide(groupe)) CreerCase();
			else {
				etat = Etat::Vide;
				cout << "Suicide" << endl;
			}

			if (p_goban->GetCaseIndispo() && doitResetCaseIndispo) p_goban->ReinitCaseIndispo();
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

	vector <vector<Case*>> tousGroupesEnnemis; //Vecteur de vecteurs qui contient sur chaque vecteur, un groupe de pierres ennemies
	vector <Case*> tempVector;

	for (int i = 0; i < a_groupe.size(); i++) {

		vector <Case*> entourage = GetEntourage(a_groupe[i]);

		for (int j = 0; j < entourage.size(); j++)
		{
			if (entourage[j]->GetEtat() == GetEtatContraire(p_goban->GetTypeJoueur()) && (!entourage[j]->GetHasBeenChecked())) {
				tempVector = GroupeDeCases(entourage[j]);

				if (!tempVector.empty()) {
					tousGroupesEnnemis.push_back(tempVector);
					tempVector.clear();
				}
			}
		}
	}

	int nbCaptures = 0;

	for (int i = 0; i < tousGroupesEnnemis.size(); i++)
	{
		for (int j = 0; j < tousGroupesEnnemis[i].size(); j++)
		{
			vector<Case*> groupeEnnemi = tousGroupesEnnemis[i];

			if (GroupeEstCapture(groupeEnnemi)) { 	//Si un groupe ennemis n'a pas de libertés, alors ça veut dire qu'on le tue donc on peut poser sa pierre
				nbCaptures++;
				for (int j = 0; j < groupeEnnemi.size(); j++) groupeEnnemi[j]->Effacer();

				if (nbCaptures == 1 && groupeEnnemi.size() == 1) p_goban->SetCaseIndispo(groupeEnnemi[0]);
			}
		}
	}

	if (nbCaptures > 0)	CaptureEffects();

	if (GroupeEstCapture(a_groupe)) return true;

	return false;
}

vector<Case*> Case::GetEntourage(Case* a_case) {
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
	vector<Case*> casesAutour = GetEntourage(this);

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

	if (p_goban->GetTypeJoueur() == Etat::Noir)		 p_goban->AjoutPointNoir();
	else if (p_goban->GetTypeJoueur() == Etat::Blanc) p_goban->AjoutPointBlanc();	

	//cout << "noir : " << p_goban->GetPtsNoir() << endl;
	//cout << "blanc : " << p_goban->GetPtsBlanc() << endl;
}

void Case::SetIndispo(bool a_indispo) { indisponibleUnTour = a_indispo; }

bool Case::GetIndispo() { return indisponibleUnTour; }