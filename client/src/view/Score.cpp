#include "view/Score.h"
#include "view/Texture.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

extern Mix_Music *gMusic;
extern Mix_Chunk *gGoalSound;

Score::Score() {
    log("Score: Creando score...", LOG_INFO);
    this->playedAlready = false;
    this->lastTimePlay = 0;
    this->soundPlayTime = 40;
    this->local = 0;
    this->visitante = 0;
    this->gFont = NULL;
    this->gFont = TTF_OpenFont("lazy.ttf", 30);
    if (this->gFont == NULL) {
        log("openLoginServer: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
}

void Score::parseYaml(YAML::Node node){
    // Local
    int gl;
    if (node["gl"]){
        gl = node["gl"].as<int>();
        if (this->local != gl) {
            golLocal();
            this->local = gl;
        }
    }
    // Visitante
    int gv;
    if (node["gv"]){
        gv = node["gv"].as<int>();
        if (this->visitante != gv) {
            golVisitante();
            this->visitante = gv;
        }
    }
}

void Score::gol() {
    if ((SDL_GetTicks() - lastTimePlay) > this->soundPlayTime){
        playedAlready = false;
    }
    if (!playedAlready)
    {
        if ( Mix_PlayingMusic() != 0 ) {
            while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()) {
            SDL_Delay(100);
            }
            Mix_PlayChannel( -1, gGoalSound, 0 );
            Mix_FadeInMusic(gMusic, -1, 1000);
        } else {
            Mix_PlayChannel( -1, gGoalSound, 0 );
        }
        playedAlready = true;
        lastTimePlay = SDL_GetTicks();
    }
}

void Score::golLocal() {
    gol();
}

void Score::golVisitante() {
    gol();
}

void Score::render(SDL_Renderer* screen) {
    // Este es el viewPort del Clock
    SDL_Rect scoreViewport;
    scoreViewport.x = 0;
    scoreViewport.y = 0;
    scoreViewport.w = 300; //SCREEN_WIDTH / 3;
    scoreViewport.h = 100;

    SDL_RenderSetViewport( screen, &scoreViewport ); //Render texture to screen
    // Dibujo los bordes del scoreViewport
    SDL_Rect outlineRect = { 0, 0, scoreViewport.w, scoreViewport.h };
    SDL_SetRenderDrawColor( screen, 0xFF, 0xFF, 0xFF, 0xFF ); //BLANCO
    SDL_RenderFillRect(screen, &outlineRect);
    SDL_SetRenderDrawColor( screen, 0x00, 0x00, 0x00, 0xFF ); //NEGRO
    SDL_RenderDrawRect( screen, &outlineRect );

    // Colores
    SDL_Color SDL_BLACK = { 0, 0, 0, 0xFF };
    SDL_Color SDL_RED = { 0xFF, 0, 0, 0xFF };

    std::string mensaje = std::to_string(this->local) + "   " + std::to_string(this->visitante);
    if (mensaje.empty()) {
      mensaje = "0";
    }

    Texture argTexture;
    argTexture.loadFromRenderedText("ARG", screen, SDL_BLACK, this->gFont);
    SDL_Rect posicionArg = {35, 10, 70, 25};
    SDL_RenderCopyEx(screen, argTexture.getSpriteSheetTexture(), NULL, &posicionArg, 0.0, NULL, SDL_FLIP_NONE);

    Texture braTexture;
    braTexture.loadFromRenderedText("BRA", screen, SDL_BLACK, this->gFont);
    SDL_Rect posicionBra = {195, 10, 70, 25};
    SDL_RenderCopyEx(screen, braTexture.getSpriteSheetTexture(), NULL, &posicionBra, 0.0, NULL, SDL_FLIP_NONE);

    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, screen, SDL_RED, this->gFont);
    SDL_Rect posicion = {35, 30, 230, 65};
    SDL_RenderCopyEx(screen, mensajeTexture.getSpriteSheetTexture(), NULL, &posicion, 0.0, NULL, SDL_FLIP_NONE);
}

Score::~Score() {
    log("Score: Liberando memoria...", LOG_INFO);
}
