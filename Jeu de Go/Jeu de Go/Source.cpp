#include "Header.h"
#include "Case.h"
#include "Goban.h"
#include "Bouton.h"

SDL_Renderer* renderer;
SDL_Surface* abandonner_btn_surface = SDL_LoadBMP("Images\\button.bmp"), *passer_btn_surface = SDL_LoadBMP("Images\\button.bmp"), *scoreBlancBackground = SDL_LoadBMP("Images\\button.bmp");
SDL_Surface* scoreNoirBackground = SDL_LoadBMP("Images\\button.bmp");
TTF_Font* police = NULL;
Mix_Music* music;
Mix_Chunk* crunchSound;
Goban* goban;
SDL_Surface* texte = NULL, * ecran = NULL, * texteScoreNoir = SDL_LoadBMP("Images\\button.bmp"), * texteScoreBlanc = SDL_LoadBMP("Images\\button.bmp");
SDL_Color noire = { 0,0,0 };
Bouton* abandonner;
Bouton* passer;
SDL_Window* window;
bool quit = false;
SDL_Event event;

int main(int argc, char** argv)
{
    InitAll();

    while (!quit) Loop();

    KillAll();
    return 0;
}

void InitAll() {
    int tailleGoban = 19;

    window = SDL_CreateWindow("Jeu de Go",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    InitSDLElements(window);
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
        Mix_PlayMusic(music, -1);
    }

    // Load sound
    crunchSound = Mix_LoadWAV("crunch.wav");
    if (!crunchSound) cout << "Error loading sound: " << Mix_GetError() << endl;
}

void InitUI() {
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    police = TTF_OpenFont("pala.ttf", 30);

    texte = TTF_RenderText_Shaded(police, "Abandonner", noire, { 255,255,255 });
    SDL_BlitSurface(texte, NULL, abandonner_btn_surface, &position);
    abandonner = new Bouton(renderer, abandonner_btn_surface, 50, 50, 200, 50);

    texte = TTF_RenderText_Shaded(police, "Passer", noire, { 255,255,255 });
    SDL_BlitSurface(texte, NULL, passer_btn_surface, &position);
    passer = new Bouton(renderer, passer_btn_surface, 500, 50, 200, 50);

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
    int tailleGoban = goban->GetTaille();

    for (int i = 0; i < tailleGoban; i++)
        for (int j = 0; j < tailleGoban; j++)
            goban->cases[i][j]->handleEvent(&event);

    if (abandonner->handleEvent(&event))  goban->Abandonner();
    else if (passer->handleEvent(&event)) goban->Passer();
}

void DisplayScore(string text, string score, SDL_Surface* scoreParent, SDL_Surface* scoreBackground, SDL_Rect a_positionScore) {


    string scoreText = text.append(score);
    char* writable = new char[scoreText.size() + 1];
    copy(scoreText.begin(), scoreText.end(), writable);
    writable[scoreText.size()] = '\0';

    scoreParent = TTF_RenderText_Shaded(police, writable, noire, { 255,255,255 });

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = 0;
    positionText.y = 0;

    SDL_BlitSurface(scoreParent, NULL, scoreBackground, &positionText);
    CreateBackground(renderer, scoreBackground, a_positionScore.x, a_positionScore.y, 200, 200);

    delete[] writable;
}

void DisplayScoreBlanc() {
    cout << "score blanc" << endl;
    SDL_Rect positionScore;
    positionScore.x = 150;
    positionScore.y = 500;

    //Arrondi à une décimale
    ostringstream out;
    out.precision(1);
    float roundedScoreValue = Round(goban->GetPtsBlanc());
    out << fixed << roundedScoreValue;

    DisplayScore("Score blanc : ", out.str(), texteScoreBlanc, scoreBlancBackground, positionScore);
}

void DisplayScoreNoir() {
    cout << "score noir" << endl;

    SDL_Rect positionScore;
    positionScore.x = 900;
    positionScore.y = 150;
    DisplayScore("Score noir : ", to_string(goban->GetPtsNoir()), texteScoreNoir, scoreNoirBackground, positionScore);
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

//Fonctions hors de la classe :

bool GroupeEstCapture(vector<Case*> a_groupe) {
    for (int i = 0; i < a_groupe.size(); i++)
        if (a_groupe[i]->HasLibertes()) return false;

    return true;
}

//Constitue le groupe liée à la pierre envoyée en param_tre
vector<Case*> GroupeDePierres(Case* a_case) {

    vector<Case*> tempVector; //Stocke pierres alliées qui n'ont pas été checkées
    vector<Case*> casesAutour = a_case->GetCasesAutour(a_case);//Récupère les cases autour de la pierre passée en paramètre

    tempVector.push_back(a_case);
    a_case->SetHasBeenChecked(true);

    for (int i = 0; i < casesAutour.size(); i++) //On parcourt les cases autour de la case passée en paramètre
    {
        Case* newCase = casesAutour[i];

        if (newCase->GetEtat() == a_case->GetEtat() && newCase->GetHasBeenChecked() == false) { //Si la case est alliée de cette passée en paramètre ET qu'elle n'a pas été checkée...
            newCase->SetHasBeenChecked(true);													//...on indique qu'elle a été checkée...
            vector <Case*> tempVector2 = GroupeDePierres(newCase);								//...on constitue depuis cette pierre...
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


