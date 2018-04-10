#include "view/PlayerSpriteManager.h"

PlayerSpriteManager::PlayerSpriteManager(Texture* spriteSheet, Player* player) {
    log("PlayerSpriteManager: Creando vista...", LOG_INFO);
    this->player = player;
    this->spriteSheet = spriteSheet;
    this->spriteWidth = 16;
    this->spriteHeight = 16;
    this->sprite = {
        0,
        48,
        this->spriteWidth,
        this->spriteHeight
    }; // Sprite actual. Esta mirando para la derecha.
    log("PlayerSpriteManager: Vista creada.", LOG_INFO);
}

// Dibuja un jugador, en las coordenadas del render que se le pase.
// Para buscar el sprite se fija la velocidad y la orientacion del jugador.
// Con eso arma un cuadrado que encierra el sprite a dibujar.
// Tambien arma otro cuadrado que representa el lugar de la pantalla (renderer)
// en el que se dibuja el sprite.
void PlayerSpriteManager::render(SDL_Renderer* screen, Coordinates* coordinates) {
    Velocity* velocity = this->player->getVelocity();
    bool sliding = this->player->isSliding();
    if (velocity->isZero() && !sliding) {
        // Esta quieto, se dibuja parado.
        this->setStandingSprite(this->player->getOrientation());
    } else {
        // Esta corriendo.
        this->setRunningSprite(velocity, sliding);
    }
    SDL_Rect positionOnScreen = this->getPositionOnScreen(this->sprite, coordinates);
    SDL_RenderCopy(
        screen,
        this->spriteSheet->getSpriteSheetTexture(),
        &this->sprite,
        &positionOnScreen
    );
}

Coordinates* PlayerSpriteManager::getPlayerCoordinates() {
    return this->player->getPosition();
}


PlayerSpriteManager::~PlayerSpriteManager() {
    log("PlayerSpriteManager: Liberando memoria. Borrando textura...", LOG_INFO);
    delete(this->spriteSheet);
    log("PlayerSpriteManager: Textura eliminada. Memoria liberada.", LOG_INFO);
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
void PlayerSpriteManager::setStandingSprite(int orientation) {
    log("PlayerSpriteManager: Creando el sprite parado.", LOG_DEBUG);
    switch (orientation) {
            case PLAYER_ORIENTATION_UP:
                this->sprite.x = 0;
                this->sprite.y = 0;
                break;
            case PLAYER_ORIENTATION_DOWN:
                this->sprite.x = 0;
                this->sprite.y = 16;
                break;
            case PLAYER_ORIENTATION_LEFT:
                this->sprite.x = 0;
                this->sprite.y = 32;
                break;
            case PLAYER_ORIENTATION_RIGHT:
                this->sprite.x = 0;
                this->sprite.y = 48;
                break;
        }
}

// Devuelve el cuadrado que encierra al sprite actual.
// Cada sprite tiene 16x16.
// Ocho sprites por secuencia de corrida.
void PlayerSpriteManager::setRunningSprite(Velocity* velocity, bool sliding) {
    int xComponent = velocity->getComponentX();
    int yComponent = velocity->getComponentY();
    if (this->isRunningUp(xComponent, yComponent)) {
        this->setRunningUpSprite(sliding);
    } else if (this->isRunningDown(xComponent, yComponent)) {
        this->setRunningDownSprite(sliding);
    } else if (this->isRunningLeft(xComponent, yComponent)) {
        this->setRunningLeftSprite(sliding);
    } else if (this->isRunningRight(xComponent, yComponent)) {
        this->setRunningRightSprite(sliding);
    }
}

bool PlayerSpriteManager::isRunningUp(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent < 0));
}

void PlayerSpriteManager::setRunningUpSprite(bool sliding) {
    // El que empieza en (0, 0) es el primero que corre para arriba.
    // El que empieza en (7 * 16 = 112, 0) es el ultimo que corre para arriba.
    if (sliding){
        this->setSlidingUpSprite();
    }
    else{
        log("PlayerSpriteManager: Creando el sprite corriendo arriba.", LOG_DEBUG);
        if ((this->sprite.x == 112) || (this->sprite.y != 0)) {
            this->sprite.x = 0; // Reinicio la secuencia.
            this->sprite.y = 0;
        } else {
            this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
        }
    }
}

bool PlayerSpriteManager::isRunningDown(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent > 0));
}

void PlayerSpriteManager::setRunningDownSprite(bool sliding) {
    // El primero es el sprite numero 13 de la segunda linea.
    // El que empieza en (0, 16) es el primero que corre para abajo.
    // El que empieza en (7 * 16 = 112, 16) es el ultimo que corre para abajo.
    if (sliding){
    this->setSlidingDownUpSprite();
    }
    else{
        log("PlayerSpriteManager: Creando el sprite corriendo abajo.", LOG_DEBUG);
        if ((this->sprite.x == 112) || (this->sprite.y != 16)) {
            // Si esta en el ultimo sprite de la secuencia, o si estoy en otra secuencia.
            // Reinicio la secuencia.
            this->sprite.x = 0;
            this->sprite.y = 16;
        } else {
            this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
        }
    }
}

bool PlayerSpriteManager::isRunningLeft(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent < 0);
}

void PlayerSpriteManager::setRunningLeftSprite(bool sliding) {
    // El primero es el sprite numero 9 de la tercera linea.
    // El que empieza en (0, 32) es el primero que corre para la izquierda.
    // El que empieza en (7 * 16 = 112, 32) es el ultimo que corre para la izquierda.
    if (sliding){
    this->setSlidingLeftSprite();
    }
    else{
        log("PlayerSpriteManager: Creando el sprite corriendo a la izquierda.", LOG_DEBUG);
        if ((this->sprite.x == 112) || (this->sprite.y != 32)) {
            this->sprite.x = 0; // Reinicio la secuencia.
            this->sprite.y = 32;
        } else {
            this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
        }
    }
}

bool PlayerSpriteManager::isRunningRight(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent > 0);
}

void PlayerSpriteManager::setRunningRightSprite(bool sliding) {
    // El que empieza en (0, 48) es el primero que corre para la derecha.
    // El que empieza en (7 * 16 = 112, 48) es el ultimo que corre para la derecha.
    if (sliding){
    this->setSlidingRightSprite();
    }
    else{
        log("PlayerSpriteManager: Creando el sprite corriendo a la derecha.", LOG_DEBUG);
        if ((this->sprite.x == 112) || (this->sprite.y != 48)) {
            this->sprite.x = 0; // Reinicio la secuencia.
            this->sprite.y = 48;
        } else {
            this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
        }
    }
}

void PlayerSpriteManager::setSlidingSprite(int orientacion) {
    if this->player->!wasSliding() {
        log("PlayerSpriteManager: Creando el sprite deslizando arriba.", LOG_DEBUG);
        this->sprite.x = 0;
        this->sprite.y = (80 + (16 * orientacion));
        /*switch (orientacion) {
            case(PLAYER_ORIENTATION_UP):
                this->sprite.x = PRIMER_X;
                this->sprite.y = PRIMER_Y;
                break;
            case(PLAYER_ORIENTATION_DOWN):
                this->sprite.x = PRIMER_X;
                this->sprite.y = PRIMER_Y;
                break;
            case(PLAYER_ORIENTATION_LEFT):
                this->sprite.x = PRIMER_X;
                this->sprite.y = PRIMER_Y;
                break;
            case(PLAYER_ORIENTATION_RIGHT):
                this->sprite.x = PRIMER_X;
                this->sprite.y = PRIMER_Y;
                break;
        }*/
        player->isAlreadySliding();
    } 
    else {
        if ((this->sprite.x == 112) || (this->sprite.y != (80 + (16 * orientacion)))) {
            this->setStandingSprite(PLAYER_ORIENTATION_UP);
            this->player->stopSliding();
        }
        else this->sprite.x += this->spriteWidth; //que es esto?
    }
}