#include "Groupe.h"

Groupe::Groupe() {
	cout << "Groupe cree" << endl;
}
Groupe::~Groupe() {
	cout << "Groupe detruit" << endl;
}
void Groupe::FusionneGroupes(Groupe groupe_a_fusionner) {
	groupe.insert(end(groupe), begin(groupe_a_fusionner.groupe), end(groupe_a_fusionner.groupe));
	groupe_a_fusionner.~Groupe();
}
