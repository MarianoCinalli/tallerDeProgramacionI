#include "view/Score.h"
#include "view/Texture.h"
#include <SDL2/SDL_ttf.h>

Score::Score() {
    log("Score: Creando score...", LOG_INFO);
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
    if (node["gl"]){
        this->local = node["gl"].as<int>();
    }
    // Visitante
    if (node["gv"]){
        this->visitante = node["gv"].as<int>();
    }
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
    SDL_Color SDL_GREEN = { 0, 0xFF, 0, 0xFF };
    SDL_Color SDL_BLUE = { 0, 0, 0xFF, 0xFF };
    SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

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
