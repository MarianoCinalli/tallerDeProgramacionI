#include "view/PlayerSpriteManager.h"

PlayerSpriteManager::PlayerSpriteManager(Texture* spriteSheet, Player* player) {
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
}

// Dibuja un jugador, en las coordenadas del render que se le pase.
// Para buscar el sprite se fija la velocidad y la orientacion del jugador.
// Con eso arma un cuadrado que encierra el sprite a dibujar.
// Tambien arma otro cuadrado que representa el lugar de la pantalla (renderer)
// en el que se dibuja el sprite.
void PlayerSpriteManager::render(SDL_Renderer* screen, Coordinates* coordinates) {
    Velocity* velocity = this->player->getVelocity();
    if (velocity->isZero()) {
        // Esta quieto, se dibuja parado.
        this->setStandingSprite(this->player->getOrientation());
    } else {
        // Esta corriendo.
        this->setRunningSprite(velocity);
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
void PlayerSpriteManager::setRunningSprite(Velocity* velocity) {
    // Este metodo se hizo feo, puede que lo refactoree.
    int xComponent = velocity->getComponentX();
    int yComponent = velocity->getComponentY();
    if (this->isRunningUp(xComponent, yComponent)) {
        this->setRunningUpSprite();
    } else if (this->isRunningDown(xComponent, yComponent)) {
        this->setRunningDownSprite();
    } else if (this->isRunningLeft(xComponent, yComponent)) {
        this->setRunningLeftSprite();
    } else if (this->isRunningRight(xComponent, yComponent)) {
        this->setRunningRightSprite();
    }
}

bool PlayerSpriteManager::isRunningUp(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent < 0));
}

void PlayerSpriteManager::setRunningUpSprite() {
    // El que empieza en (0, 0) es el primero que corre para arriba.
    // El que empieza en (7 * 16 = 112, 0) es el ultimo que corre para arriba.
    log("PlayerSpriteManager: Creando el sprite corriendo arriba.", LOG_DEBUG);
    if ((this->sprite.x == 112) || (this->sprite.y != 0)) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 0;
    } else {
        this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
    }
}

bool PlayerSpriteManager::isRunningDown(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent > 0));
}

void PlayerSpriteManager::setRunningDownSprite() {
    // El primero es el sprite numero 13 de la segunda linea.
    // El que empieza en (0, 16) es el primero que corre para abajo.
    // El que empieza en (7 * 16 = 112, 16) es el ultimo que corre para abajo.
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

bool PlayerSpriteManager::isRunningLeft(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent < 0);
}

void PlayerSpriteManager::setRunningLeftSprite() {
    // El primero es el sprite numero 9 de la tercera linea.
    // El que empieza en (0, 32) es el primero que corre para la izquierda.
    // El que empieza en (7 * 16 = 112, 32) es el ultimo que corre para la izquierda.
    log("PlayerSpriteManager: Creando el sprite corriendo a la izquierda.", LOG_DEBUG);
    if ((this->sprite.x == 112) || (this->sprite.y != 32)) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 32;
    } else {
        this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
    }
}

bool PlayerSpriteManager::isRunningRight(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent > 0);
}

void PlayerSpriteManager::setRunningRightSprite() {
    // El que empieza en (0, 48) es el primero que corre para la derecha.
    // El que empieza en (7 * 16 = 112, 48) es el ultimo que corre para la derecha.
    log("PlayerSpriteManager: Creando el sprite corriendo a la derecha.", LOG_DEBUG);
    if ((this->sprite.x == 112) || (this->sprite.y != 48)) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 48;
    } else {
        this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
    }
}
