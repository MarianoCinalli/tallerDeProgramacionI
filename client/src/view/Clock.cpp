#include "view/Clock.h"
#include "view/Texture.h"
#include <SDL2/SDL_ttf.h>

Clock::Clock() {
    log("Clock: Creando clock...", LOG_INFO);
    this->value = "00-00";
    this->gFont = NULL;
    this->gFont = TTF_OpenFont("lazy.ttf", 30);
    if (this->gFont == NULL) {
        log("openLoginServer: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
}

void Clock::parseYaml(YAML::Node node){
    if (node["clock"]){
        this->value = node["clock"].as<std::string>();
    }
}

void Clock::render(SDL_Renderer* screen) {
    // Este es el viewPort del Clock
    SDL_Rect clockViewport;
    clockViewport.x = 500; //2 * (SCREEN_WIDTH / 3);
    clockViewport.y = 0;
    clockViewport.w = 300; //SCREEN_WIDTH / 3;
    clockViewport.h = 100;

    SDL_RenderSetViewport( screen, &clockViewport ); //Render texture to screen
    SDL_Rect outlineRect = { 0, 0, clockViewport.w, clockViewport.h };
    SDL_SetRenderDrawColor( screen, 0xFF, 0xFF, 0xFF, 0xFF ); //BLANCO
    // Dibujo los bordes del clockViewport
    SDL_RenderFillRect(screen, &outlineRect);
    SDL_SetRenderDrawColor( screen, 0x00, 0x00, 0x00, 0xFF ); //NEGRO
    SDL_RenderDrawRect( screen, &outlineRect );

    // Colores
    SDL_Color SDL_BLACK = { 0, 0, 0, 0xFF };
    SDL_Color SDL_RED = { 0xFF, 0, 0, 0xFF };

    std::string mensaje = this->value;
    if (mensaje.empty()) {
      mensaje = "00-00";
    }
    std::replace( mensaje.begin(), mensaje.end(), '-', ':');
    int minutos = stoi(mensaje.substr(0, mensaje.find(":")));
    //segundos = stoi(mensaje.substr(mensaje.find(":")+1,mensaje.length()));
    std::string mensajeTime;
    if ( minutos < 45 ) {
      mensajeTime = "PRIMER TIEMPO";
    } else {
      mensajeTime = "SEGUNDO TIEMPO";
    }

    Texture timeTexture;
    timeTexture.loadFromRenderedText(mensajeTime, screen, SDL_BLACK, this->gFont);
    SDL_Rect posicionTime = {20, 10, 260, 25};
    SDL_RenderCopyEx(screen, timeTexture.getSpriteSheetTexture(), NULL, &posicionTime, 0.0, NULL, SDL_FLIP_NONE);

    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, screen, SDL_RED, this->gFont);
    SDL_Rect posicion = {35, 30, 230, 65};
    SDL_RenderCopyEx(screen, mensajeTexture.getSpriteSheetTexture(), NULL, &posicion, 0.0, NULL, SDL_FLIP_NONE);
}

Clock::~Clock() {
    log("Clock: Liberando memoria...", LOG_INFO);
}
