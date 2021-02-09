#pragma once
#include "Header.h"

class Goban;

bool GroupeEstCapture(vector<Case*> a_groupe);
vector<Case*> GroupeDeCases(Case* a_case);

//Fonctions de debug
void AfficherCoordonnees(Case* _case);
void AfficherEtat(Etat a_etat);
void AfficherVecteur(vector<Case*> a_vecteur);


class Case
{
protected:
	Etat etat;

	pair<int, int> positionInPx;
	pair<int, int> coordonnees;

	Goban* p_goban;

	SDL_Texture* caseTexture;

	SDL_Renderer* p_renderer;
	SDL_Window* p_window;

	bool hasBeenChecked = false;

	bool indisponibleUnTour = false;
public:
	Case();
	Case(Goban* goban, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* caseTexture, int a_index, int a_tailleGoban);

	~Case();

	void CreerCase();

	void handleEvent(SDL_Event* e);

	void SetIndex(int a_index, int a_tailleGoban);
	pair<int, int> GetIndex();

	void SetPositionInPx(int x, int y);
	std::pair<int, int> GetPositionInPx();

	void SetEtat(Etat a_etat);
	Etat GetEtatContraire(Etat a_etat);
	Etat GetEtat();

	bool CaseEstVide();

	bool SeSuicide(vector<Case*> a_groupe);

	bool HasLibertes();

	vector<Case*> GetEntourage(Case* a_case);

	void SetHasBeenChecked(bool a_hasBeenChecked);
	bool GetHasBeenChecked();
	
	void SetTexture(SDL_Texture* caseTexture);
	SDL_Texture* GetTexture();

	void SetIndispo(bool a_indispo);
	bool GetIndispo();

	void Effacer();
};



