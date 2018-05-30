#include "view/BallSpriteManager.h"

BallSpriteManager::BallSpriteManager(Texture* spriteSheet, Ball* ball) {
    log("BallSpriteManager: Creando vista...", LOG_INFO);
    this->ball = ball;
    this->dibujoConOffset = false;
    this->rollingCount = 0;
    this->spriteSheet = spriteSheet;
    this->sprite = {
        0,
        29 * SPRITE_SIZE,
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
    SDL_Rect renderQuad = {
        coordinates->getX(),
        coordinates->getY(),
        SPRITE_SIZE,
        SPRITE_SIZE
    };
    return renderQuad;
}

void BallSpriteManager::setMovingBallSprite() {
    if (rollingCount == 1024) { //TODO hardcode value
        rollingCount = 0;
    }
    int level = this->ball->getHeight();
    if (level == 0) {
        int orientation = (this->ball->getOrientation());
        int orientationOffset = 0;
        if (this->dibujoConOffset) {      //Para que dibuje una con offset y una no
            switch (orientation) {
                case PLAYER_ORIENTATION_UP: 
                    orientationOffset = 1;
                    break;
                case PLAYER_ORIENTATION_RIGHT: 
                    orientationOffset = 2;
                    break;
                case PLAYER_ORIENTATION_DOWN: 
                    orientationOffset = 3;
                    break;
                case PLAYER_ORIENTATION_LEFT: 
                    orientationOffset = 4;
                    break;
            }
        }
        if ((rollingCount % ROLLING_DIVISOR) == 0) {
            log("BallSpriteManager: Creando el sprite del balon.", LOG_SPAM);
            if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != ( 21 + orientationOffset) * SPRITE_SIZE)) {
                this->sprite.x = 0; // Reinicio la secuencia.
                this->sprite.y = ((21 + orientationOffset) * SPRITE_SIZE);
                this->cambioOffset();
            } else {
                this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
            }
        }
    }
    else {
        if ((0 < level) && (level < 5)) {
            if ((rollingCount % ROLLING_DIVISOR) == 0) {
                log("BallSpriteManager: Creando el sprite del balon aereo.", LOG_SPAM);
                if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != ( 25 + level) * SPRITE_SIZE)) {
                    this->sprite.x = 0; // Reinicio la secuencia.
                    this->sprite.y = ((25 + level) * SPRITE_SIZE);
                } else {
                    this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
                }
            }    
        }
        else {
            log("BallSpriteManager: Problema con nivel de pase aereo.", LOG_SPAM);
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