#include "Header.h"
#include "Case.h"
#include "Goban.h"
#include "Bouton.h"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Surface* abandonner_btn_surface = SDL_LoadBMP("Images\\button.bmp"), *passer_btn_surface = SDL_LoadBMP("Images\\button.bmp"), *rejouerBouton_surface = SDL_LoadBMP("Images\\button.bmp"),
*scoreBlancBackground = SDL_LoadBMP("Images\\background_score.bmp"), *scoreNoirBackground = SDL_LoadBMP("Images\\background_score.bmp"),
*backgroundTexteWinner = SDL_LoadBMP("Images\\background_score.bmp"), *background = SDL_LoadBMP("Images\\background.bmp");

SDL_Rect scoreSize;

TTF_Font *police = NULL;
Mix_Music *music;
Mix_Chunk *crunchSound;
SDL_Surface* texte = NULL, * ecran = NULL, *texteWinner = SDL_LoadBMP("Images\\button.bmp");
SDL_Color noir = { 0,0,0 };

SDL_Event event;

Goban* goban;

bool quit = false;

Bouton* abandonner, * passer, * rejouerBouton;

bool game = true;
bool choix = true;
vector<Bouton*> boutonsChoix;
vector<int> InitUITaille();
void FreeChoiceScreen(vector<Bouton*> a_boutons);

int ChoiceScreen() {
    vector<int> boutonsValues = InitUITaille();

    SDL_WaitEvent(&event);

    for (size_t i = 0; i < boutonsValues.size(); i++)
        if (boutonsChoix[i]->handleEvent(&event)) {
            choix = false;
            return boutonsValues[i];
        }
}

bool jeuEncours = true;

int main(int argc, char** argv)
{
    while (jeuEncours) {
        InitAll();

        int tailleChoisie = 0;


        while (choix) tailleChoisie = ChoiceScreen();

        FreeChoiceScreen(boutonsChoix);

        InitGame(tailleChoisie);

        while (!quit) LoopGame();
    }


    KillAll();
    return 0;
}


vector<int> InitUITaille() {
    police = TTF_OpenFont("pala.ttf", 70);

    vector<int> taillesGoban = { 9, 13, 15, 19};

    for (int i = 0; i < taillesGoban.size(); i++)
    {
        SDL_Surface* newBoutonSurface = SDL_LoadBMP("Images\\button.bmp");

        int marginTop = 300;
        int margeY_entreBoutons = 10;
        int taille = taillesGoban[i];

        SDL_Rect tailleBouton;
        tailleBouton.x = 200;
        tailleBouton.y = 60;

        Bouton* bouton = new Bouton(renderer, newBoutonSurface, to_string(taille),SCREEN_WIDTH / 2 - newBoutonSurface->w / 4, i * (tailleBouton.y + margeY_entreBoutons) + marginTop, tailleBouton.x, tailleBouton.y);
        
        boutonsChoix.push_back(bouton);
    }

    return taillesGoban;
}

void InitGame(int a_tailleGoban) {
    CreateBackground(renderer, background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    goban = new Goban(renderer, window, a_tailleGoban);
    InitUI();
}

void FreeChoiceScreen(vector<Bouton*> a_boutons) {
    SDL_RenderClear(renderer);
    a_boutons.clear();
    quit = false;
}

int DetermineTailleGoban() {
    int taille = 0;

    while (taille < 3 || taille > 19) {
        system("cls");
        cout << "Entrez taille goban : ";
        cin >> taille;
    }

    return taille;
}

void InitAll() {
    window = SDL_CreateWindow("Jeu de Go",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    InitSDLElements(window);

    police = TTF_OpenFont("pala.ttf", 70);

    SDL_Surface* question = TTF_RenderText_Blended(police, "Choisir taille du goban :", noir);

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = SCREEN_WIDTH /2 - question->w / 4 + 50;
    positionText.y = 150;

    SDL_BlitSurface(question, NULL, background, &positionText);

    CreateBackground(renderer, background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_FreeSurface(question);

    background = SDL_LoadBMP("Images\\background.bmp");

    InitMusic();
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


    //Bouton abandonner

    positionBouton.x = (SCREEN_WIDTH / 2) - (abandonner_btn_surface->w / 4);
    positionBouton.y = 10;

    abandonner = new Bouton(renderer, abandonner_btn_surface, "Abandonner", positionBouton.x, positionBouton.y, 200, 50);


    //Bouton passer

    positionBouton.y += passer_btn_surface->h / 2;

    passer = new Bouton(renderer, passer_btn_surface, "Passer", positionBouton.x, positionBouton.y, 200, 50);



    scoreSize.x = 250;
    scoreSize.y = 250;

    police = TTF_OpenFont("pala.ttf", 50);

    DisplayScoreBlanc(scoreSize.x, scoreSize.y);
    DisplayScoreNoir(scoreSize.x, scoreSize.y);
}

void InitSDLElements(SDL_Window* window) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Error initializing SDL: " << SDL_GetError() << endl;

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (TTF_Init() == -1)
    {
        cout << "Erreur d'initialisation de TTF_Init :" << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }
}

void  Rejouer() {
    SDL_RenderClear(renderer);
    choix = true;
    quit = true;
    game = true;
    goban->~Goban();
    SDL_DestroyWindow(window);
}

void LoopGame() {
    SDL_WaitEvent(&event);

    if (event.type == SDL_QUIT) {
        quit = true;
        jeuEncours = false;
    }

    if (game) {
        int tailleGoban = goban->GetTaille();

        for (int i = 0; i < tailleGoban; i++)
            for (int j = 0; j < tailleGoban; j++)
                goban->cases[i][j]->handleEvent(&event);

        if (abandonner->handleEvent(&event))  goban->Abandonner();
        else if (passer->handleEvent(&event)) goban->Passer();
    }
    else {
        if(rejouerBouton->handleEvent(&event)) Rejouer();
    }

}

void DisplayScore(string text, string score, SDL_Surface* _textScore, SDL_Surface* _scoreBackground, SDL_Rect positionSurface, int a_sizeBackGroundX, int a_sizeBackGroundY) {

    string scoreText = text.append(score);
    char* writable = new char[scoreText.size() + 1];
    copy(scoreText.begin(), scoreText.end(), writable);
    writable[scoreText.size()] = '\0';

    SDL_Surface* textScore = TTF_RenderText_Blended(police, writable, noir);
    SDL_Surface* scoreBackground = SDL_LoadBMP("Images\\background_score.bmp");

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = scoreBackground->w / 2 - textScore->w / 2;
    positionText.y = scoreBackground->h / 2 - textScore->h / 2;

    SDL_BlitSurface(textScore, NULL, scoreBackground, &positionText);
    CreateBackground(renderer, scoreBackground, positionSurface.x, positionSurface.y, a_sizeBackGroundX, a_sizeBackGroundY);

    SDL_FreeSurface(textScore);
    delete[] writable;
}

void DisplayScoreBlanc(int a_sizeBackGroundX, int a_sizeBackGroundY) {
    //Arrondi à une décimale

    int margin = 15;
    pair<int, int> positionCase = goban->cases[goban->GetTaille() - 1][0]->GetPositionInPx();

    SDL_Rect positionSurface;
    positionSurface.x = positionCase.first + CASES_SIZE + margin;
    positionSurface.y = positionCase.second;
    SDL_Surface* texteScoreBlanc = SDL_LoadBMP("Images\\button.bmp");

    DisplayScore("Score blanc : ", Round(goban->GetPtsBlanc()), texteScoreBlanc, scoreBlancBackground, positionSurface, a_sizeBackGroundX, a_sizeBackGroundY);
}

void DisplayScoreNoir(int a_sizeBackGroundX, int a_sizeBackGroundY) {
    int margin = 15;
    pair<int, int> positionCase = goban->cases[0][goban->GetTaille() - 1]->GetPositionInPx();

    SDL_Rect positionSurface;
    positionSurface.x = positionCase.first - a_sizeBackGroundX - margin;
    positionSurface.y = positionCase.second + CASES_SIZE - a_sizeBackGroundY;
    SDL_Surface* texteScoreNoir = SDL_LoadBMP("Images\\button.bmp");
    DisplayScore("Score noir : ", to_string(goban->GetPtsNoir()), texteScoreNoir, scoreNoirBackground, positionSurface, a_sizeBackGroundX, a_sizeBackGroundY);
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

    if (goban->GetTypeJoueur() == Etat::Noir) DisplayScoreNoir(scoreSize.x, scoreSize.y);
    else                                      DisplayScoreBlanc(scoreSize.x, scoreSize.y);

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

string Round(float value) { 
    string result = to_string(((float)((int)(value * 10))) / 10);

    bool rencontrePoint = false;

    for (int i = 0; i < result.size(); i++) {
        if (result[i] == '.') rencontrePoint = true;
        else if (result[i] == '0' && rencontrePoint) result[i] = NULL;
    }
        
    return result; 
}

bool GroupeEstCapture(vector<Case*> a_groupe) {
    for (int i = 0; i < a_groupe.size(); i++)
        if (a_groupe[i]->HasLibertes()) return false;

    return true;
}


//Constitue le groupe liée à la pierre envoyée en paramètre
vector<Case*> GroupeDeCases(Case* a_case) {

    vector<Case*> tempVector; //Stocke pierres alliées qui n'ont pas été checkées
    vector<Case*> casesAutour = a_case->GetEntourage();//Récupère les cases autour de la pierre passée en paramètre

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

        vector<Case*> entourageNewCase = newCase->GetEntourage();

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

                        DisplayScoreNoir(scoreSize.x, scoreSize.y);
                    }
                    else if(etatEntourageGroupe == Etat::Blanc){
                        for (int i = 0; i < tempVector.size(); i++) goban->AjoutPointBlanc();

                        DisplayScoreBlanc(scoreSize.x, scoreSize.y);
                    }
            }
        }
    }
}

void FinPartie() {
    game = false;
    ComptagePoints();



    AfficherWinner(texteWinner, backgroundTexteWinner);
}

void AfficherWinner(SDL_Surface* textParent, SDL_Surface* background) {
    string winner;

    if (goban->GetPtsBlanc() > goban->GetPtsNoir()) winner = "Blanc";
    else winner = "Noir";

    string text = " a gagné";

    string winText = winner.append(text);
    char* writable = new char[winText.size() + 1];
    copy(winText.begin(), winText.end(), writable);
    writable[winText.size()] = '\0';

    textParent = TTF_RenderText_Blended(police, writable, noir);

    int size = 300;

    //Position de l'encadré
    SDL_Rect pos;
    pos.x = SCREEN_WIDTH / 2 - size / 2;
    pos.y = SCREEN_HEIGHT / 2 - size /2;

    cout << textParent->w << endl;

    //Position du texte dans son parent
    SDL_Rect positionText;
    positionText.x = size / 2 - 90;
    positionText.y = size / 2;

    cout << positionText.x << endl;



    SDL_BlitSurface(textParent, NULL, background, &positionText);
    CreateBackground(renderer, background, pos.x, pos.y, size, size);

    rejouerBouton = new Bouton(renderer, rejouerBouton_surface, "Rejouer", pos.x + size / 2 - rejouerBouton_surface->w / 4, pos.y + size / 2 + 50, 200, 50);

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


