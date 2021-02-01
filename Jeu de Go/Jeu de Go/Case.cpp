//Si on veut utiliser les méthodes d'une classe il faut inclure son header. Sinon on peut juste inclure 'class nomDeLaClasse'.

#include "Case.h"
#include "Fonctions.h"
#include "Goban.h"

class Goban;

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
Case::~Case() { cout << "Case x : "  + to_string(coordonnees.first) + " y :" + to_string(coordonnees.second) + " detruite" << endl; }


//Gérer le clique sur les cases
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

		if (x <= thisCaseX)			 inside = false;
		else if (x > thisCaseX + CASES_SIZE) inside = false;
		else if (y <= thisCaseY)			 inside = false;
		else if (y > thisCaseY + CASES_SIZE) inside = false;

		if (inside) {
			p_goban->ResetHasBeenCheckedParameter();

			bool auTourDeNoir = p_goban->GetNoirDoitJouer();

			//cout << "x : " << GetIndex().first << " y : " << GetIndex().second << endl;

			if (GetPeutPoser(auTourDeNoir)) {

				if (auTourDeNoir) etat = Etat::Noir;
				else			  etat = Etat::Blanc;

				p_goban->SetTourJoueur();

				SDL_Surface* newSurface = SDL_LoadBMP("Images\\vide.bmp");

				switch (etat) {
				case Etat::Noir:
					newSurface = SDL_LoadBMP("Images\\noir.bmp");
					break;
				case Etat::Blanc:
					newSurface = SDL_LoadBMP("Images\\blanc.bmp");
					break;

				}

				SDL_DestroyTexture(GetTexture());

				SDL_Texture* newTexture = SDL_CreateTextureFromSurface(p_renderer, newSurface);
				if (inside) SetTexture(newTexture);
			}
		}
	}
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
void Case::SetEtat(Etat a_etat) { etat = a_etat; }


//Si le joueur qui doit jouer peut le faire sur cette case
void Case::SetPeutPoser(bool a_peutPoser) { a_peutPoser = !a_peutPoser; }

bool Case::GetPeutPoser(bool a_noirJoue) {
	if (NbCasesVides_NbCasesEnnemies().first == 0) return false;
	if (etat != Etat::Vide) return false;
	return true;
}


/// <summary>
/// first = nombre de cases vides autour de celle-ci - 
/// second = nombre de cases ennemies autour de celle-ci
/// </summary>
/// <returns></returns>
pair<int, int> Case::NbCasesVides_NbCasesEnnemies() {
	vector<Case*> casesAutour;

	SetHasBeenChecked(true);

	int x = GetIndex().first;
	int y = GetIndex().second;

	int tailleGoban = p_goban->GetTaille();


	if (x - 1 >= 0)			 casesAutour.push_back(p_goban->cases[x - 1][y]);
	if (x + 1 < tailleGoban) casesAutour.push_back(p_goban->cases[x + 1][y]);
	if (y - 1 >= 0)			 casesAutour.push_back(p_goban->cases[x][y - 1]);
	if (y + 1 < tailleGoban) casesAutour.push_back(p_goban->cases[x][y + 1]);

	int nbVides = 0;
	int nbCasesAdverses = 0;

	bool noirDoitJouer = p_goban->GetNoirDoitJouer();

	for (int i = 0; i < casesAutour.size(); i++)
	{
		Case* _case = casesAutour[i];

		//printf("x : %d y : %d :\n", _case->GetIndex().first, _case->GetIndex().second);

		switch (_case->GetEtat()) {
		case Etat::Vide: 

			if (!_case->GetHasBeenChecked()) {
				nbVides++;
				_case->SetHasBeenChecked(true);
			}
			break;

		case Etat::Noir:
			if (!noirDoitJouer) {  //Case noire est ennemie
				if (!_case->GetHasBeenChecked()) {
					nbCasesAdverses++;
					_case->SetHasBeenChecked(true);
				}
			}
			else {
				if (!_case->GetHasBeenChecked()/* && nbVides == 0*/) {
					pair<int, int> returnValues = _case->NbCasesVides_NbCasesEnnemies();
					nbVides += returnValues.first;
					nbCasesAdverses += returnValues.second;
				}
			}
			break;

		case Etat::Blanc:

			if (noirDoitJouer) {  //Case noire est ennemie
				if (!_case->GetHasBeenChecked()) {
					nbCasesAdverses++;
					_case->SetHasBeenChecked(true);
				}
			}
			else {
				if (!_case->GetHasBeenChecked()/* && nbVides == 0*/) {
					pair<int, int> returnValues = _case->NbCasesVides_NbCasesEnnemies();
					nbVides += returnValues.first;
					nbCasesAdverses += returnValues.second;
				}
			}
			break;
		}
	}

	cout << nbVides << " " << nbCasesAdverses << endl;

	pair<int, int> outValues;

	outValues.first = nbVides;
	outValues.second = nbCasesAdverses;



	return outValues;
}

//Si on a déjà fait des vérifications sur cette case
void Case::SetHasBeenChecked(bool a_hasBeenChecked) { hasBeenChecked = a_hasBeenChecked; }
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