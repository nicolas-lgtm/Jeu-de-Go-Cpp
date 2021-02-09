#include "Header.h"
#include "Case.h"
#include "Goban.h"
#include "Bouton.h"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Surface* abandonner_btn_surface = SDL_LoadBMP("Images\\button.bmp"), *passer_btn_surface = SDL_LoadBMP("Images\\button.bmp"), 
*scoreBlancBackground = SDL_LoadBMP("Images\\button.bmp"), *scoreNoirBackground = SDL_LoadBMP("Images\\button.bmp"),
*backgroundTexteWinner = SDL_LoadBMP("Images\\button.bmp");

TTF_Font *police = NULL;
Mix_Music *music;
Mix_Chunk *crunchSound;
SDL_Surface* texte = NULL, * ecran = NULL, *texteScoreNoir = SDL_LoadBMP("Images\\button.bmp"), *texteScoreBlanc = SDL_LoadBMP("Images\\button.bmp"), 
*texteWinner = SDL_LoadBMP("Images\\button.bmp");
SDL_Color noir = { 0,0,0 };

SDL_Event event;

Goban* goban;

bool quit = false;

Bouton* abandonner;
Bouton* passer;

bool game = true;


int main(int argc, char** argv)
{
    InitAll();

    while (!quit) Loop();

    KillAll();
    return 0;
}

void InitAll() {

    window = SDL_CreateWindow("Jeu de Go",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    InitSDLElements(window);

    int tailleGoban = 19;
    goban = new Goban(renderer, window, tailleGoban);

    InitMusic();
    InitUI();
}

void InitMusic() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        cout << Mix_GetError() << endl;
    }
    else {
        music = Mix_LoadMUS("menu.wav");
        //Mix_PlayMusic(music, -1);
    }

    // Load sound
    crunchSound = Mix_LoadWAV("crunch.wav");
    if (!crunchSound) cout << "Error loading sound: " << Mix_GetError() << endl;
}

void InitUI() {

    police = TTF_OpenFont("pala.ttf", 50);

    SDL_Rect positionBouton;
    SDL_Rect positionTextBouton;


    //Bouton abandonner

    positionBouton.x = (SCREEN_WIDTH / 2) - (abandonner_btn_surface->w / 4);
    positionBouton.y = 10;

    texte = TTF_RenderText_Shaded(police, "Abandonner", noir, { 255,255,255 });

    positionTextBouton.x = abandonner_btn_surface->w / 2 - texte->w / 2;
    positionTextBouton.y = abandonner_btn_surface->h / 2 - texte->h / 2;


    SDL_BlitSurface(texte, NULL, abandonner_btn_surface, &positionTextBouton);
    abandonner = new Bouton(renderer, abandonner_btn_surface, positionBouton.x, positionBouton.y, 200, 50);


    //Bouton passer

    positionBouton.y += passer_btn_surface->h / 2;

    texte = TTF_RenderText_Shaded(police, "Passer", noir, { 255,255,255 });

    positionTextBouton.x = passer_btn_surface->w / 2 - texte->w / 2;
    positionTextBouton.y = passer_btn_surface->h / 2 - texte->h / 2;

    SDL_BlitSurface(texte, NULL, passer_btn_surface, &positionTextBouton);
    passer = new Bouton(renderer, passer_btn_surface, positionBouton.x, positionBouton.y, 200, 50);

    police = TTF_OpenFont("pala.ttf", 40);

    DisplayScoreBlanc();
    DisplayScoreNoir();
}

void InitSDLElements(SDL_Window* window) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Error initializing SDL: " << SDL_GetError() << endl;

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (TTF_Init() == -1)
    {
        cout << "Erreur d'initialisation de TTF_Init :" << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    //InitSprites();
}

void Loop() {
    SDL_WaitEvent(&event);

    if (event.type == SDL_QUIT) quit = true;

    if (game) {
        int tailleGoban = goban->GetTaille();

        for (int i = 0; i < tailleGoban; i++)
            for (int j = 0; j < tailleGoban; j++)
                goban->cases[i][j]->handleEvent(&event);

        if (abandonner->handleEvent(&event))  goban->Abandonner();
        else if (passer->handleEvent(&event)) goban->Passer();
    }

}

void DisplayScore(string text, string score, SDL_Surface* textScore, SDL_Surface* scoreBackground, SDL_Rect positionSurface) {


    string scoreText = text.append(score);
    char* writable = new char[scoreText.size() + 1];
    copy(scoreText.begin(), scoreText.end(), writable);
    writable[scoreText.size()] = '\0';

    textScore = TTF_RenderText_Shaded(police, writable, noir, { 255,255,255 });

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = scoreBackground->w / 2 - textScore->w / 2;
    positionText.y = scoreBackground->h / 2 - textScore->h / 2;

    SDL_BlitSurface(textScore, NULL, scoreBackground, &positionText);
    CreateBackground(renderer, scoreBackground, positionSurface.x, positionSurface.y, 350, 100);

    delete[] writable;
}

void DisplayScoreBlanc() {
    //Arrondi à une décimale
    //ostringstream out;
    //out.precision(1);
    //float roundedScoreValue = Round(goban->GetPtsBlanc());
    //out << fixed << roundedScoreValue;

    int margin = 15;
    pair<int, int> positionCase = goban->cases[goban->GetTaille() - 1][0]->GetPositionInPx();

    SDL_Rect positionSurface;
    positionSurface.x = positionCase.first + CASES_SIZE + margin;
    positionSurface.y = positionCase.second - scoreBlancBackground->h + CASES_SIZE + 3;

    DisplayScore("Score blanc : ", to_string(goban->GetPtsBlanc()), texteScoreBlanc, scoreBlancBackground, positionSurface);
}

void DisplayScoreNoir() {
    int margin = 15;
    pair<int, int> positionCase = goban->cases[0][goban->GetTaille() - 1]->GetPositionInPx();

    SDL_Rect positionSurface;
    positionSurface.x = positionCase.first - scoreNoirBackground->w + CASES_SIZE * 3 - margin - 3;
    positionSurface.y = positionCase.second + CASES_SIZE;

    DisplayScore("Score noir : ", to_string(goban->GetPtsNoir()), texteScoreNoir, scoreNoirBackground, positionSurface);
}

void CreateBackground(SDL_Renderer* a_renderer, SDL_Surface* a_background, int a_x, int a_y, int a_sizeX, int a_sizeY) {
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(a_renderer, a_background);
    SDL_Rect dstrect = { a_x, a_y, a_sizeX, a_sizeY };
    SDL_RenderCopy(a_renderer, newTexture, NULL, &dstrect);
    SDL_RenderPresent(a_renderer);
}

void CreateButton(SDL_Renderer* a_renderer, SDL_Surface* a_bouton, int a_x, int a_y, int a_sizeX, int a_sizeY) { CreateBackground(a_renderer, a_bouton, a_x, a_y, a_sizeX, a_sizeY); }

void CaptureEffects() {
    Mix_PlayChannel(-1, crunchSound, 0);

    if (goban->GetTypeJoueur() == Etat::Noir) DisplayScoreNoir();
    else                                      DisplayScoreBlanc();

    goban->SetCapture(false);
}

void KillAll() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_FreeSurface(texte);
    SDL_Quit();
}

float Round(float value) { return ((float)((int)(value * 10))) / 10; }

bool GroupeEstCapture(vector<Case*> a_groupe) {
    for (int i = 0; i < a_groupe.size(); i++)
        if (a_groupe[i]->HasLibertes()) return false;

    return true;
}


vector<Case*> GetCasesAutour(Case* a_case) {
    vector<Case*> casesAutour;

    int x = a_case->GetIndex().first;
    int y = a_case->GetIndex().second;

    int tailleGoban = goban->GetTaille();

    if (x - 1 >= 0)			 casesAutour.push_back(goban->cases[x - 1][y]);
    if (x + 1 < tailleGoban) casesAutour.push_back(goban->cases[x + 1][y]);
    if (y - 1 >= 0)			 casesAutour.push_back(goban->cases[x][y - 1]);
    if (y + 1 < tailleGoban) casesAutour.push_back(goban->cases[x][y + 1]);

    return casesAutour;
}

//Constitue le groupe liée à la pierre envoyée en paramètre
vector<Case*> GroupeDeCases(Case* a_case) {

    vector<Case*> tempVector; //Stocke pierres alliées qui n'ont pas été checkées
    vector<Case*> casesAutour = a_case->GetEntourage(a_case);//Récupère les cases autour de la pierre passée en paramètre

    tempVector.push_back(a_case);
    a_case->SetHasBeenChecked(true);

    for (int i = 0; i < casesAutour.size(); i++) //On parcourt les cases autour de la case passée en paramètre
    {
        Case* newCase = casesAutour[i];

        if (newCase->GetEtat() == a_case->GetEtat() && newCase->GetHasBeenChecked() == false) { //Si la case est alliée de cette passée en paramètre ET qu'elle n'a pas été checkée...
            newCase->SetHasBeenChecked(true);													//...on indique qu'elle a été checkée...
            vector <Case*> tempVector2 = GroupeDeCases(newCase);								//...on constitue depuis cette pierre...
            tempVector.insert(end(tempVector), begin(tempVector2), end(tempVector2));			//...on ajoute le groupe de cette pierre au groupe de pierre déjà existant.
        }
    }

    return tempVector; //Retourne le groupe constitué
}

Etat CheckEtatEntourageGroupe(vector<Case*> a_groupeDeCases) {

    Etat etatEntourageGroupe = Etat::Vide;

    for (int i = 0; i < a_groupeDeCases.size(); i++)
    {
        Case* newCase = a_groupeDeCases[i];

        vector<Case*> entourageNewCase = newCase->GetEntourage(newCase);

        for (int j = 0; j < entourageNewCase.size() ; j++)
        {
            Case* newCaseEntourage = entourageNewCase[j];

            if (newCaseEntourage->GetEtat() != Etat::Vide) {
                if (etatEntourageGroupe == Etat::Vide)
                    etatEntourageGroupe = newCaseEntourage->GetEtat();
                else if (etatEntourageGroupe != Etat::Vide && newCaseEntourage->GetEtat() != etatEntourageGroupe) 
                    return Etat::Vide;
            }
        }
    }

    return etatEntourageGroupe;
}

void ComptagePoints() {
    goban->ResetHasBeenCheckedParameter();
    int tailleGoban = goban->GetTaille();

    vector<Case*> tempVector;

    for (int i = 0; i < tailleGoban; i++)
    {
        for (int j = 0; j < tailleGoban; j++)
        {
            Case* newCase = goban->cases[i][j];

            if (newCase->GetEtat() == Etat::Vide && !newCase->GetHasBeenChecked()) {
                tempVector = GroupeDeCases(newCase);
                AfficherEtat(CheckEtatEntourageGroupe(tempVector));

                Etat etatEntourageGroupe = CheckEtatEntourageGroupe(tempVector);

                cout << tempVector.size() << endl;

                    if (etatEntourageGroupe == Etat::Noir) {
                        for (int i = 0; i < tempVector.size(); i++) goban->AjoutPointNoir();

                        DisplayScoreNoir();
                    }
                    else if(etatEntourageGroupe == Etat::Blanc){
                        for (int i = 0; i < tempVector.size(); i++) goban->AjoutPointBlanc();

                        DisplayScoreBlanc();
                    }
            }
        }
    }
}

void FinPartie() {
    game = false;
    ComptagePoints();

    SDL_Rect pos;
    pos.x = 300;
    pos.y = 300;

    AfficherWinner(texteWinner, backgroundTexteWinner, pos);
}

void AfficherWinner(SDL_Surface* textParent, SDL_Surface* background, SDL_Rect a_position) {
    string winner;

    if (goban->GetPtsBlanc() > goban->GetPtsNoir()) winner = "Blanc";
    else winner = "Noir";

    string text = " a gagné";

    string winText = winner.append(text);
    char* writable = new char[winText.size() + 1];
    copy(winText.begin(), winText.end(), writable);
    writable[winText.size()] = '\0';

    textParent = TTF_RenderText_Shaded(police, writable, noir, { 255,255,255 });

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = 0;
    positionText.y = 0;

    SDL_BlitSurface(textParent, NULL, background, &positionText);
    CreateBackground(renderer, background, a_position.x, a_position.y, 200, 200);

    delete[] writable;
}

//Fonctions de debug :
void AfficherVecteur(vector<Case*> a_vecteur) {
    for (int i = 0; i < a_vecteur.size(); i++)
        AfficherCoordonnees(a_vecteur[i]);

    cout << endl << endl;
}
void AfficherCoordonnees(Case* _case) { cout << "x : " << _case->GetIndex().first << " y : " << _case->GetIndex().second << endl; }
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


