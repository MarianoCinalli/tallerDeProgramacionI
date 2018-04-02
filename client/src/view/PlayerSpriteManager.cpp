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
    log("PlayerSpriteManager: Dibujando sprite: " + this->getSpriteAsText(), LOG_DEBUG);
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
    log("PlayerSpriteManager: Creando el sprite parado.", LOG_DEBUG);
    newSprite = {
        0,
        0,
        this->spriteWidth,
        this->spriteHeight
    };
    return newSprite;
}

// Devuelve el cuadrado que encierra al sprite actual.
// Cada sprite tiene 16x16.
SDL_Rect PlayerSpriteManager::getRunningSprite(Velocity* velocity) {
    // Este metodo se hizo feo, puede que lo refactoree.
    int xComponent = velocity->getComponentX();
    int yComponent = velocity->getComponentY();
    if (xComponent != 0) {
        if (xComponent > 0) {
            // Ocho sprites por secuencia de corrida.
            // El que empieza en (0, 48) es el primero que corre para la derecha.
            // El que empieza en (7 * 16 = 112, 48) es el ultimo que corre para la derecha.
            log("PlayerSpriteManager: Creando el sprite corriendo.", LOG_DEBUG);
            if ((this->sprite.x == 112) || (this->sprite.y != 48)) {
                // Si esta en el ultimo sprite de la secuencia, o si estoy en otra secuencia.
                // Reinicio la secuencia.
                this->sprite.x = 0;
                this->sprite.y = 48;
            } else {
                this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
            }
        } else {
            // Ocho sprites por secuencia de corrida.
            // El primero es el sprite numero 9 de la tercera linea.
            // El que empieza en (0, 32) es el primero que corre para la izquierda.
            // El que empieza en (7 * 16 = 112, 32) es el ultimo que corre para la izquierda.
            log("PlayerSpriteManager: Creando el sprite corriendo.", LOG_DEBUG);
            if ((this->sprite.x == 112) || (this->sprite.y != 32)) {
                this->sprite.x = 0; // Reinicio la secuencia.
                this->sprite.y = 32;
            } else {
                this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
            }
        }
    }
    if (yComponent != 0) {
        if (yComponent > 0) {
            // Ocho sprites por secuencia de corrida.
            // El primero es el sprite numero 13 de la segunda linea.
            // El que empieza en (0, 16) es el primero que corre para abajo.
            // El que empieza en (7 * 16 = 112, 16) es el ultimo que corre para abajo.
            log("PlayerSpriteManager: Creando el sprite corriendo.", LOG_DEBUG);
            if ((this->sprite.x == 112) || (this->sprite.y != 16)) {
                this->sprite.x = 0; // Reinicio la secuencia.
                this->sprite.y = 16;
            } else {
                this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
            }
        } else {
            // Ocho sprites por secuencia de corrida.
            // El que empieza en (0, 0) es el primero que corre para arriba.
            // El que empieza en (7 * 16 = 112, 0) es el ultimo que corre para arriba.
            log("PlayerSpriteManager: Creando el sprite corriendo.", LOG_DEBUG);
            if ((this->sprite.x == 112) || (this->sprite.y != 0)) {
                this->sprite.x = 0; // Reinicio la secuencia.
                this->sprite.y = 0;
            } else {
                this->sprite.x += this->spriteWidth; // Avanzo la secuencia en un frame.
            }
        }
    }
    return this->sprite; // Esto tambien es temporal.
}

// Por ahora la uso para debugear.
std::string PlayerSpriteManager::getSpriteAsText() {
    return "x: " + std::to_string(this->sprite.x) + " y: " + std::to_string(this->sprite.y);
}
