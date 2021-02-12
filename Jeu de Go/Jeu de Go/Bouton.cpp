#include "Bouton.h"
#include "Header.h"

Bouton::Bouton(SDL_Renderer* a_renderer, SDL_Surface* a_bouton, string a_texte, int a_x, int a_y, int a_sizeX, int a_sizeY) {
	SetPosition(a_x, a_y);
	SetSize(a_sizeX, a_sizeY);
	SetSurface(a_bouton);

	TTF_Font* police = TTF_OpenFont("pala.ttf", 70);
	SDL_Surface* texte = NULL;
	SDL_Rect positionTextBouton;

	char* writable = new char[a_texte.size() + 1];
	copy(a_texte.begin(), a_texte.end(), writable);
	writable[a_texte.size()] = '\0';

	texte = TTF_RenderText_Blended(police, writable, { 0, 0, 0 });

	positionTextBouton.x = a_bouton->w / 2 - texte->w / 2;
	positionTextBouton.y = a_bouton->h / 2 - texte->h / 2;

	SDL_BlitSurface(texte, NULL, a_bouton, &positionTextBouton);

	CreateBackground(a_renderer, a_bouton, a_x, a_y, a_sizeX, a_sizeY);
}

Bouton::~Bouton() {

}

void Bouton::SetSurface(SDL_Surface* a_surface) {
	surface = a_surface;
}

SDL_Surface* Bouton::GetSurface() {
	return surface;
}


void Bouton::SetPosition(int a_x, int a_y) {
	position.x = a_x;
	position.y = a_y;
}

SDL_Rect Bouton::GetPosition() {
	return position;
}

void Bouton::SetSize(int a_x, int a_y) {
	size.x = a_x;
	size.y = a_y;
}

SDL_Rect Bouton::GetSize() {
	return size;
}


bool Bouton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Rect position = GetPosition();
		SDL_Rect size = GetSize();

		int boutonPosX = position.x;
		int boutonPosY = position.y;

		//Check if mouse is in button
		bool inside = true;

		if      (x <= boutonPosX)		  inside = false;
		else if (x > boutonPosX + size.x) inside = false;
		else if (y <= boutonPosY)		  inside = false;
		else if (y > boutonPosY + size.y) inside = false;

		return inside;
	}
	else {
		return false;
	}

}

