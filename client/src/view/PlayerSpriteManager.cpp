#include "view/PlayerSpriteManager.h"
#include <SDL2/SDL_mixer.h>
#include "util/Constants.h"

extern Mix_Chunk *gKickSound;

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
    this->standingCount = 0;
    this->kickingCount = 0;
    this->slidingCount = 0;
    this->runningCount = 0;
    this->activePlayerCount = 0;
    log("PlayerSpriteManager: Vista creada.", LOG_INFO);
}

// Dibuja un jugador, en las coordenadas del render que se le pase.
// Para buscar el sprite se fija la velocidad y la orientacion del jugador.
// Con eso arma un cuadrado que encierra el sprite a dibujar.
// Tambien arma otro cuadrado que representa el lugar de la pantalla (renderer)
// en el que se dibuja el sprite.

double calculateDegrees(int orientation) {
        return (orientation-1)*45;
    }

void PlayerSpriteManager::render(SDL_Renderer* screen, Coordinates* coordinates) {
    bool still = this->player->isStill();
    bool sliding = this->player->isSliding();
    bool kicking = this->player->isKicking();
    if (!kicking) {
        this->stopKicking();
    }
    if (!sliding) {
        this->stopSliding();
    }
    bool runningFast = this->player->isRunningFast();
    int orientation = this->player->getOrientation();
    if (still && (!sliding) && (!kicking)) {
        // Esta quieto, se dibuja parado.
        this->setStandingSprite(orientation);
    } else {
        this->setSprite(sliding, kicking, runningFast);
    }

    SDL_Rect positionOnScreen = this->getPositionOnScreen(this->sprite, coordinates);
    SDL_Texture* spriteSheet = this->spriteSheet->getSpriteSheetTexture();

    double degrees = 0;
    if ((orientation % 2) == 0){
      degrees = 45;
    }
    if (sliding || kicking){
        degrees = calculateDegrees(orientation);
    }

    SDL_RenderCopyEx(
        screen,
        spriteSheet,
        &this->sprite,
        &positionOnScreen,
        degrees,
        NULL,
        SDL_FLIP_NONE
    );
    // Active player marker.
    if (this->player->getIsSelected() && (this->player->userName != "NONE")) {
        // log("PLAYER SPRITE MANAGER: orientacion: ",this->player->getOrientation(), LOG_DEBUG);

        SDL_Rect markerSprite = this->getActivePlayerMarker();
        SDL_Rect markerPositionOnScreen = this->getActivePlayerMarkerPosition(coordinates);

        SDL_RenderCopy(
            screen,
            spriteSheet,
            &markerSprite,
            &markerPositionOnScreen
        );
    }
    // SDL_RenderSetScale(screen,1,1);

    
}

int PlayerSpriteManager::getPlayerTeam() {
  return this->player->getTeam();
}

void PlayerSpriteManager::setSprite(bool sliding, bool kicking, bool runningFast) {
    if (sliding || kicking) {
        if (sliding)
            this->setSlidingSprite();
        else
            this->setKickingSprite();
    } else {
        setRunningSprite(runningFast);
    }
}

Coordinates* PlayerSpriteManager::getPlayerCoordinates() {
    return this->player->getPosition();
}


PlayerSpriteManager::~PlayerSpriteManager() {
    log("BallSpriteManager: Borrando PlayerSpriteManager.", LOG_INFO);
}

// Metodos privados. -------------------------------------------------------------------------

// Devuelve el cuadrado que va a ocupar el sprite en la pantalla.
SDL_Rect PlayerSpriteManager::getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates) {
    SDL_Rect renderQuad = {
        coordinates->getX() - DRAW_SIZE_CORRECTION,
        coordinates->getY() - DRAW_SIZE_CORRECTION,
        DRAW_SIZE_PLAYER,
        DRAW_SIZE_PLAYER
    };
    return renderQuad;
}


// Standing sprite --------------------------------------------------------------------------

// Devuelve el cuadrado que encierra al sprite actual.
// Por ahora solo mira para arriba.
void PlayerSpriteManager::setStandingSprite(int orientation) {
    if (standingCount == 1024) { //TODO hardcode value
        standingCount = 0;
    }
    if ((standingCount % STANDING_DIVISOR) == 0) {
        switch (orientation) {
            case PLAYER_ORIENTATION_UP:
                this->setStandingSpriteViewUp();
                break;
            case PLAYER_ORIENTATION_UP_R:
                this->setStandingSpriteViewUp();
                break;
            case PLAYER_ORIENTATION_RIGHT:
                this->setStandingSpriteViewRight();
                break;
            case PLAYER_ORIENTATION_DOWN_R:
                this->setStandingSpriteViewRight();
                break;
            case PLAYER_ORIENTATION_DOWN:
                this->setStandingSpriteViewDown();
                break;
            case PLAYER_ORIENTATION_DOWN_L:
                this->setStandingSpriteViewDown();
                break;
            case PLAYER_ORIENTATION_LEFT:
                this->setStandingSpriteViewLeft();
                break;
            case PLAYER_ORIENTATION_UP_L:
                this->setStandingSpriteViewLeft();
                break;
        }
    }
    standingCount += 1;
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
void PlayerSpriteManager::setRunningSprite(bool runningFast) {
    if (runningCount == 1024) { //TODO hardcode value
        runningCount = 0;
    }
    this->stopKicking();
    this->stopSliding();
    if ((runningCount % RUNNING_DIVISOR) == 0) {
        int cteVelocidad = 0;
        if (runningFast) {
            cteVelocidad = 17;
        }
        int orientation = this->player->getOrientation();
        switch (orientation) {
            case PLAYER_ORIENTATION_UP:
                this->setRunningUpSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_UP_R:
                this->setRunningUpSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_RIGHT:
                this->setRunningRightSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_DOWN_R:
                this->setRunningRightSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_DOWN:
                this->setRunningDownSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_DOWN_L:
                this->setRunningDownSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_LEFT:
                this->setRunningLeftSprite(cteVelocidad);
                break;
            case PLAYER_ORIENTATION_UP_L:
                this->setRunningLeftSprite(cteVelocidad);
                break;
        }
    }
    runningCount += 1;
}

bool PlayerSpriteManager::isRunningUp(int xComponent, int yComponent) {
    return ((xComponent == 0) && (yComponent < 0));
}

void PlayerSpriteManager::setRunningUpSprite(int cteVelocidad) {
    log("PlayerSpriteManager: Creando el sprite corriendo arriba.", LOG_SPAM);
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
    log("PlayerSpriteManager: Creando el sprite corriendo a la derecha.", LOG_SPAM);
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
    log("PlayerSpriteManager: Creando el sprite corriendo abajo.", LOG_SPAM);
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
    log("PlayerSpriteManager: Creando el sprite corriendo a la izquierda.", LOG_SPAM);
    if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (3 * SPRITE_SIZE) + (SPRITE_SIZE * cteVelocidad))) {
        this->sprite.x = 0; // Reinicio la secuencia.
        this->sprite.y = 3 * SPRITE_SIZE + (SPRITE_SIZE * cteVelocidad);
    } else {
        this->sprite.x += SPRITE_SIZE; // Avanzo la secuencia en un frame.
    }
}

//model


void PlayerSpriteManager::startsKicking() {
    if (!this->sliding) {
        this->kicking = true;
    }
}

void PlayerSpriteManager::stopKicking() {
    // this->kicking = false;
    this->wasKicking = false;
}

bool PlayerSpriteManager::isKicking() {
    return this->kicking;
}

bool PlayerSpriteManager::wasKickingYet() {
    return this->wasKicking;
}

void PlayerSpriteManager::isAlreadyKicking() {
    this->wasKicking = true;
    // Kick sound
    Mix_PlayChannel( -1, gKickSound, 0 );
}

bool PlayerSpriteManager::isSliding() {
    return this->sliding;
}

bool PlayerSpriteManager::wasSlidingYet() {
    return this->wasSliding;
}

void PlayerSpriteManager::startsSliding() {
    if (!this->kicking) {
        this->sliding = true;
    }
    //this->velocity->accelerate(this->orientation, this->maxVelocity);
}

void PlayerSpriteManager::isAlreadySliding() {
    this->wasSliding = true;
}

void PlayerSpriteManager::stopSliding() {
    // this->sliding = false;
    this->wasSliding = false;
}

// Kicking --------------------------------------------------------------------------

void PlayerSpriteManager::setKickingSprite() {
    if (kickingCount == 1024) { //TODO hardcode value
        kickingCount = 0;
    }
    if ((kickingCount % KICKING_DIVISOR) == 0) {
        int orientation = 1;
        if (!this->wasKickingYet()) {
            log("PlayerSpriteManager: Creando el sprite pateando arriba.", LOG_SPAM);
            this->sprite.x = 0;
            this->sprite.y = (3 * SPRITE_SIZE + (SPRITE_SIZE * orientation));
            this->isAlreadyKicking();
        } else {
            if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (3 * SPRITE_SIZE + (SPRITE_SIZE * orientation)))) {
                // this->setStandingSprite(orientation);
                this->stopKicking();
            } else {
                this->sprite.x += SPRITE_SIZE;
            }
        }
    }
    kickingCount += 1;
}

// Sliding --------------------------------------------------------------------------

void PlayerSpriteManager::setSlidingSprite() {
    if (slidingCount == 1024) { //TODO hardcode value
        slidingCount = 0;
    }
    if ((slidingCount % SLIDING_DIVISOR) == 0) {
        int orientation = 1;
        if (!this->wasSlidingYet()) {
            log("PlayerSpriteManager: Creando el sprite deslizando arriba.", LOG_SPAM);
            this->sprite.x = 0;
            this->sprite.y = (7 * SPRITE_SIZE + (SPRITE_SIZE * orientation));
            this->isAlreadySliding();
        } else {
            if ((this->sprite.x == 3 * SPRITE_SIZE) || (this->sprite.y != (7 * SPRITE_SIZE + (SPRITE_SIZE * orientation)))) {
                // this->setStandingSprite(orientation);
                this->stopSliding();
            } else {
                this->sprite.x += SPRITE_SIZE;
            }
        }
    }
    slidingCount += 1;
}

// Active player marker ----------------------------
SDL_Rect PlayerSpriteManager::getActivePlayerMarker() {
    if (activePlayerCount == 1024) { //TODO hardcode value
        activePlayerCount = 0;
    }
    if ((activePlayerCount % ACTIVE_PLAYER_DIVISOR) == 0) {
        if (this->activePlayerMarker.y == 16 * SPRITE_SIZE) {
            int size = 0;
            std::string name = this->player->userName;
            if (name == "juan") {
                size = 1;
            } else if (name == "claudio") {
                size = 1;
            } else if (name == "mariano") {
                size = 3;
            } else if (name == "nicolas") {
                size = 2;
            } else if (name == "joaquin") {
                size = 2;
            } else if (name == "NONE") {
                activePlayerCount++;
                this->activePlayerMarker = {
                    size * SPRITE_SIZE,
                    16 * SPRITE_SIZE,
                    0,
                    0
                };
                return this->activePlayerMarker;
            }
            this->activePlayerMarker = {
                size * SPRITE_SIZE,
                16 * SPRITE_SIZE,
                SPRITE_SIZE,
                SPRITE_SIZE
            };
        }
    }
    activePlayerCount++;
    return this->activePlayerMarker;
}

SDL_Rect PlayerSpriteManager::getActivePlayerMarkerPosition(Coordinates* coordinates) {
    return {
        coordinates->getX() - DRAW_SIZE_CORRECTION + 1,
        coordinates->getY() - DRAW_SIZE_PLAYER,
        DRAW_SIZE_PLAYER,
        DRAW_SIZE_PLAYER
    };
}
