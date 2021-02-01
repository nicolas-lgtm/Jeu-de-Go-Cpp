#pragma once
#include "Header.h"
class Goban;

class Case
{
protected:
	Etat etat;
	pair<int, int> positionInPx;
	pair<int, int> coordonnees;
	bool peutJouer;

	Goban* p_goban;

	SDL_Texture* caseTexture;

	SDL_Renderer* p_renderer;
	SDL_Window* p_window;

	bool hasBeenChecked = false;

public:
	Case();
	Case(Goban* goban, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* caseTexture, int a_index, int a_tailleGoban);

	~Case();

	void handleEvent(SDL_Event* e);

	void SetIndex(int a_index, int a_tailleGoban);
	pair<int, int> GetIndex();

	void SetPositionInPx(int x, int y);
	std::pair<int, int> GetPositionInPx();

	void SetEtat(Etat a_etat);
	Etat GetEtat();

	void SetPeutPoser(bool a_peutPoser);
	bool GetPeutPoser(bool a_noirJoue);

	pair<int, int> NbCasesVides_NbCasesEnnemies();
	void SetHasBeenChecked(bool a_hasBeenChecked);
	bool GetHasBeenChecked();

	void SetTexture(SDL_Texture* caseTexture);
	SDL_Texture* GetTexture();
};



