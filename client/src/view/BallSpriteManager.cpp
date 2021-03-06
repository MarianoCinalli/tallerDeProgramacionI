#include "view/BallSpriteManager.h"

BallSpriteManager::BallSpriteManager(Texture* spriteSheet, Ball* ball) {
    log("BallSpriteManager: Creando vista...", LOG_INFO);
    this->ball = ball;
    this->dibujoConOffset = false;
    this->rollingCount = 0;
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
    if (!this->ball->isStill()) {
        this->setMovingBallSprite();
    }
    else {
        this->setStillBall();
    }
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
    log("BallSpriteManager: Vista eliminada.", LOG_INFO);
}

// Metodos privados. -------------------------------------------------------------------------

// Devuelve el cuadrado que va a ocupar el sprite en la pantalla.
SDL_Rect BallSpriteManager::getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates) {
    int height = this->ball->getHeight();
    SDL_Rect renderQuad = {
        coordinates->getX(),
        coordinates->getY(),
        SPRITE_SIZE + 10*height,
        SPRITE_SIZE + 10*height
    };
    return renderQuad;
}

void BallSpriteManager::setStillBall() {
    this->sprite.x = 0;
    this->sprite.y = (21 * SPRITE_SIZE);
}

void BallSpriteManager::setMovingBallSprite() {
    if (rollingCount == 1024) { //TODO hardcode value
        rollingCount = 0;
    }
    if ((rollingCount % ROLLING_DIVISOR) == 0) {
        log("BallSpriteManager: Creando el sprite del balon aereo.", LOG_SPAM);
        if ((this->sprite.x == 3 * SPRITE_SIZE)){ // || (this->sprite.y != (25 + level) * SPRITE_SIZE)) {
            this->sprite.x = 0; // Reinicio la secuencia.
        }
        else {
            this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
        }
    }
    rollingCount ++;
}

void BallSpriteManager::cambioOffset() {
    if (this->dibujoConOffset) {
        this->dibujoConOffset = false;
    }
    else {
        this->dibujoConOffset = true;
    }
}
