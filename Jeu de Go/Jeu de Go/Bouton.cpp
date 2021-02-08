#include "Bouton.h"
#include "Header.h"

Bouton::Bouton(SDL_Renderer* a_renderer, SDL_Surface* a_bouton, int a_x, int a_y, int a_sizeX, int a_sizeY) {
	CreateBackground(a_renderer, a_bouton, a_x, a_y, a_sizeX, a_sizeY);
	SetPosition(a_x, a_y);
	SetSize(a_sizeX, a_sizeY);
}

Bouton::~Bouton() {

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

