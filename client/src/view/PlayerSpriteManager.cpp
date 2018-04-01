#include "view/PlayerSpriteManager.h"

PlayerSpriteManager::PlayerSpriteManager(SpriteSheet* spriteSheet) {
    this->spriteSheet = spriteSheet;
    this->spriteWidth = 16;
    this->spriteHeight = 16;
    this->sprite = {
        0,
        0,
        this->spriteWidth,
        this->spriteHeight
    }; // Sprite actual.
}

// Arma el sprite y lo dibuja en el renderer.
// Para buscar el sprite se fija la velocidad y la orientacion del jugador.
// Con eso arma un cuadrado que encierra el sprite a dibujar.
// Tambien arma otro cuadrado que representa el lugar de la pantalla (renderer)
// en el que se dibuja el sprite.
void PlayerSpriteManager::render(Player* player, SDL_Renderer* screen) {
    Velocity* velocity = player->getVelocity();
    if (velocity->isZero()) {
        // Esta quieto, se dibuja parado.
        int orientation = player->getOrientation();
        this->sprite = this->getStandingSprite(orientation);
    } else {
        // Esta corriendo.
        this->sprite = this->getRunningSprite(velocity);
    }
    Coordinates* coordinates = player->getPosition();
    SDL_Rect positionOnScreen = this->getPositionOnScreen(this->sprite, coordinates);
    SDL_RenderCopy(
        screen,
        this->spriteSheet->getSpriteSheetTexture(),
        &this->sprite,
        &positionOnScreen
    );
}

PlayerSpriteManager::~PlayerSpriteManager() {
    
}

// Metodos privados.

// Devuelve el cuadrado que va a ocupar el sprite en la pantalla.
SDL_Rect PlayerSpriteManager::getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates) {
    SDL_Rect renderQuad = {
        coordinates->getX(),
        coordinates->getY(),
        this->spriteWidth,
        this->spriteHeight
    };
    return renderQuad;
}

// Devuelve el cuadrado que encierra al sprite actual.
// Por ahora solo mira para arriba.
SDL_Rect PlayerSpriteManager::getStandingSprite(int orientation) {
    SDL_Rect newSprite;
    newSprite = {
        0,
        0,
        this->spriteWidth,
        this->spriteHeight
    };
    return newSprite;
}

// Devuelve el cuadrado que encierra al sprite actual.
// Por ahora solo corre en una direccion (arriba).
SDL_Rect PlayerSpriteManager::getRunningSprite(Velocity* velocity) {
    // Ocho sprites por secuencia de corrida.
    // Cada sprite tiene 16x16.
    // El que empieza en (0, 0) es el primero que corre para arriba.
    // El que empieza en (0, 72) es el ultimo que corre para arriba.  
    if (this->sprite.x == 72) {
        this->sprite.x = 0; // Reinicio la secuencia.
    } else {
        this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
    }
    return this->sprite; // Esto tambien es temporal.
}
