#pragma once
#include "Header.h"

class Groupe
{
protected:
	vector<Case*> groupe;

public:
	Groupe();
	~Groupe();

	void FusionneGroupes(Groupe groupe_a_fusionner);
};

