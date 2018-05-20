#include "view/PlayerSpriteManager.h"

PlayerSpriteManager::PlayerSpriteManager(Texture* spriteSheet, Player* player) {
    log("PlayerSpriteManager: Creando vista...", LOG_INFO);
    this->player = player;
    this->spriteSheet = spriteSheet;
    this->sprite = {
        4 * SPRITE_SIZE,
        SPRITE_SIZE,
        SPRITE_SIZE,
        SPRITE_SIZE
    }; // Sprite actual. Esta mirando para la derecha.
    this->activePlayerMarker = {
        0,
        16 * SPRITE_SIZE,
        SPRITE_SIZE,
        SPRITE_SIZE
    };
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
    bool kicking = this->player->isKicking();
    bool runningFast = this->player->isRunningFast();
    if (velocity->isZero() && (!sliding) && (!kicking)) {
        // Esta quieto, se dibuja parado.
        this->setStandingSprite(this->player->getOrientation());
    } else {
        this->setSprite(velocity, sliding, kicking, runningFast);
    }
    SDL_Rect positionOnScreen = this->getPositionOnScreen(this->sprite, coordinates);
    SDL_Texture* spriteSheet = this->spriteSheet->getSpriteSheetTexture();
    SDL_RenderCopy(
        screen,
        spriteSheet,
        &this->sprite,
        &positionOnScreen
    );
    // Active player marker.
    if (this->player->getIsSelected()) {
        SDL_Rect markerSprite = this->getActivePlayerMarker();
        SDL_Rect markerPositionOnScreen = this->getActivePlayerMarkerPosition(coordinates);
        SDL_RenderCopy(
            screen,
            spriteSheet,
            &markerSprite,
            &markerPositionOnScreen
        );
    }
}

void PlayerSpriteManager::setSprite(Velocity* velocity, bool sliding, bool kicking, bool runningFast) {
    if (sliding || kicking) {
        if (sliding)
            this->setSlidingSprite();
        else
            this->setKickingSprite();
    } else {
        setRunningSprite(velocity, runningFast);
    }
}

Coordinates* PlayerSpriteManager::getPlayerCoordinates() {
    return this->player->getPosition();
}


PlayerSpriteManager::~PlayerSpriteManager() {
}

// Metodos privados. -------------------------------------------------------------------------

// Devuelve el cuadrado que va a ocupar el sprite en la pantalla.
SDL_Rect PlayerSpriteManager::getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates) {
    SDL_Rect renderQuad = {
        coordinates->getX(),
        coordinates->getY(),
        SPRITE_SIZE,
        SPRITE_SIZE
    };
    return renderQuad;
}

// Standing sprite --------------------------------------------------------------------------

// Devuelve el cuadrado que encierra al sprite actual.
// Por ahora solo mira para arriba.
void PlayerSpriteManager::setStandingSprite(int orientation) {
    // log("PlayerSpriteManager: Creando el sprite parado.", LOG_DEBUG);
    switch (orientation) {
        case PLAYER_ORIENTATION_UP:
            this->setStandingSpriteViewUp();
            break;
        case PLAYER_ORIENTATION_RIGHT:
            this->setStandingSpriteViewRight();
            break;
        case PLAYER_ORIENTATION_DOWN:
            this->setStandingSpriteViewDown();
            break;
        case PLAYER_ORIENTATION_LEFT:
            this->setStandingSpriteViewLeft();
            break;
    }
}

void PlayerSpriteManager::setStandingSpriteViewUp() {
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != 12 * SPRITE_SIZE)) {
        this->sprite.x = 0;
        this->sprite.y = 12 * SPRITE_SIZE;
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

void PlayerSpriteManager::setStandingSpriteViewRight() {
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != 13 * SPRITE_SIZE)) {
        this->sprite.x = 0;
        this->sprite.y = 13 * SPRITE_SIZE;
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

void PlayerSpriteManager::setStandingSpriteViewDown() {
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != 14 * SPRITE_SIZE)) {
        this->sprite.x = 0;
        this->sprite.y = 14 * SPRITE_SIZE;
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

void PlayerSpriteManager::setStandingSpriteViewLeft() {
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != 15 * SPRITE_SIZE)) {
        this->sprite.x = 0;
        this->sprite.y = 15 * SPRITE_SIZE;
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

// Running sprite --------------------------------------------------------------------------

// Devuelve el cuadrado que encierra al sprite actual.
// Cada sprite tiene SPRITE_SIZE x SPRITE_SIZE.
// Cuatro sprites por secuencia de corrida.
void PlayerSpriteManager::setRunningSprite(Velocity* velocity, bool runningFast) {
    int xComponent = velocity->getComponentX();
    int yComponent = velocity->getComponentY();
    int cteVelocidad = 0;
    if (runningFast) {
        cteVelocidad = 17;
    }
    if (this->isRunningUp(xComponent, yComponent)) {
        this->setRunningUpSprite(cteVelocidad);
    } else if (this->isRunningDown(xComponent, yComponent)) {
        this->setRunningDownSprite(cteVelocidad);
    } else if (this->isRunningLeft(xComponent, yComponent)) {
        this->setRunningLeftSprite(cteVelocidad);
    } else if (this->isRunningRight(xComponent, yComponent)) {
        this->setRunningRightSprite(cteVelocidad);
    }
}

bool PlayerSpriteManager::isRunningUp(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent < 0));
}

void PlayerSpriteManager::setRunningUpSprite(int cteVelocidad) {
    log("PlayerSpriteManager: Creando el sprite corriendo arriba.", LOG_DEBUG);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (0 + (SPRITE_SIZE * cteVelocidad)))) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 0 + (SPRITE_SIZE * cteVelocidad);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

bool PlayerSpriteManager::isRunningRight(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent > 0);
}

void PlayerSpriteManager::setRunningRightSprite(int cteVelocidad) {
    log("PlayerSpriteManager: Creando el sprite corriendo a la derecha.", LOG_DEBUG);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != SPRITE_SIZE + (SPRITE_SIZE * cteVelocidad))) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = SPRITE_SIZE + (SPRITE_SIZE * cteVelocidad);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

bool PlayerSpriteManager::isRunningDown(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent > 0));
}

void PlayerSpriteManager::setRunningDownSprite(int cteVelocidad) {
    log("PlayerSpriteManager: Creando el sprite corriendo abajo.", LOG_DEBUG);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (2 * SPRITE_SIZE) + (SPRITE_SIZE * cteVelocidad))) {
        // Si esta en el ultimo sprite de la secuencia, o si estoy en otra secuencia.
        // Reinicio la secuencia.
        this->sprite.x = 0;
        this->sprite.y = (2 * SPRITE_SIZE) + (SPRITE_SIZE * cteVelocidad);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

bool PlayerSpriteManager::isRunningLeft(int xComponent, int yComponent) {
    // Si se mueve en diagonal tambien va este.
    return (xComponent < 0);
}

void PlayerSpriteManager::setRunningLeftSprite(int cteVelocidad) {
    log("PlayerSpriteManager: Creando el sprite corriendo a la izquierda.", LOG_DEBUG);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (3 * SPRITE_SIZE) + (SPRITE_SIZE * cteVelocidad))) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 3 * SPRITE_SIZE + (SPRITE_SIZE * cteVelocidad);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

// Kicking --------------------------------------------------------------------------

void PlayerSpriteManager::setKickingSprite() {
    int orientation = this->player->getOrientation();
    if (!this->player->wasKickingYet()) {
        log("PlayerSpriteManager: Creando el sprite pateando arriba.", LOG_DEBUG);
        this->sprite.x = 0;
        this->sprite.y = (3 * SPRITE_SIZE + (SPRITE_SIZE * orientation));
        this->player->isAlreadyKicking();
    } else {
        if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (3 * SPRITE_SIZE + (SPRITE_SIZE * orientation)))) {
            this->setStandingSprite(orientation);
            this->player->stopKicking();
        } else {
            this->sprite.x += SPRITE_SIZE;
        }
    }
}

// Sliding --------------------------------------------------------------------------

void PlayerSpriteManager::setSlidingSprite() {
    int orientation = this->player->getOrientation();
    if (!this->player->wasSlidingYet()) {
        log("PlayerSpriteManager: Creando el sprite deslizando arriba.", LOG_DEBUG);
        this->sprite.x = 0;
        this->sprite.y = (7 * SPRITE_SIZE + (SPRITE_SIZE * orientation));
        this->player->isAlreadySliding();
    } else {
        if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (7 * SPRITE_SIZE + (SPRITE_SIZE * orientation)))) {
            this->setStandingSprite(orientation);
            this->player->stopSliding();
        } else {
            this->sprite.x += SPRITE_SIZE;
        }
    }
}
// Active player marker ----------------------------

SDL_Rect PlayerSpriteManager::getActivePlayerMarker() {
    if ((this->activePlayerMarker.x == 0) && (this->activePlayerMarker.y == 16 * SPRITE_SIZE)) {
        this->activePlayerMarker = {
            SPRITE_SIZE,
            16 * SPRITE_SIZE,
            SPRITE_SIZE,
            SPRITE_SIZE
        };
    } else {
        this->activePlayerMarker = {
            0,
            16 * SPRITE_SIZE,
            SPRITE_SIZE,
            SPRITE_SIZE
        };
    }
    return this->activePlayerMarker;
}

SDL_Rect PlayerSpriteManager::getActivePlayerMarkerPosition(Coordinates* coordinates) {
    return {
        coordinates->getX(),
        coordinates->getY() - SPRITE_SIZE,
        SPRITE_SIZE,
        SPRITE_SIZE
    };
}
