#include "view/BallSpriteManager.h"

BallSpriteManager::BallSpriteManager(Texture* spriteSheet, Ball* ball) {
    log("BallSpriteManager: Creando vista...", LOG_INFO);
    this->ball = ball;
    this->spriteSheet = spriteSheet;
    this->sprite = {
        0,
        21 * SPRITE_SIZE,
        SPRITE_SIZE,
        SPRITE_SIZE
    };
    log("BallSpriteManager: Vista creada.", LOG_INFO);
}


Coordinates* BallSpriteManager::getBallCoordinates() {
    return this->ball->getPosition();
}

void BallSpriteManager::render(SDL_Renderer* screen, Coordinates* coordinates) {


    this->setMovingBallSprite();
    // Coordinates* coordinates = this->getBallCoordinates();
    SDL_Rect positionOnScreen = this->getPositionOnScreen(this->sprite, coordinates);
    SDL_Texture* spriteSheet = this->spriteSheet->getSpriteSheetTexture();
    SDL_RenderCopy(
        screen,
        spriteSheet,
        &this->sprite,
        &positionOnScreen
    );
}



BallSpriteManager::~BallSpriteManager() {
}

// Metodos privados. -------------------------------------------------------------------------

// Devuelve el cuadrado que va a ocupar el sprite en la pantalla.
SDL_Rect BallSpriteManager::getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates) {
    SDL_Rect renderQuad = {
        coordinates->getX(),
        coordinates->getY(),
        SPRITE_SIZE,
        SPRITE_SIZE
    };
    return renderQuad;
}

void BallSpriteManager::setMovingBallSprite() {
    log("BallSpriteManager: Creando el sprite del balon.", LOG_DEBUG);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != 21 * SPRITE_SIZE)) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = (21 * SPRITE_SIZE);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}
