#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height, int margen) {
    log("Camera: Creando camara...", LOG_INFO);
    this->position = position;
    this->width = width;
    this->height = height;
    //The margin area
    this->margin = { position->getX() + margen, position->getY() + margen, width - margen * 2, height - margen * 2 };
    log("Camera: Camara creada.", LOG_INFO);
}

Coordinates* Camera::getRelativeCoordinates(Coordinates* absolutePosition) {
    Coordinates* relativePosition = new Coordinates(
        absolutePosition->getX() - position->getX(),
        absolutePosition->getY() - position->getY()
    );
    return relativePosition;
}

void Camera::calculateNewPostion(Coordinates* playerPosition) {
    // Margen derecho
    if ((playerPosition->getX() + SPRITE_SIZE) > (margin.x + margin.w)) {
        log("Jugador activo a la derecha del margen derecho", LOG_DEBUG);
        this->position->addX(10);
        margin.x = this->position->getX() + MARGIN_SIZE;
    }

    // Margen izquierdo
    if (playerPosition->getX() < margin.x) {
        log("Jugador activo a la izquierda del margen izquierdo", LOG_DEBUG);
        this->position->subtractX(10);
        margin.x = this->position->getX() + MARGIN_SIZE;
    }

    // Margen inferior
    if ((playerPosition->getY() + SPRITE_SIZE) > (margin.y + margin.h)) {
        log("Jugador activo debajo del margen inferior", LOG_DEBUG);
        this->position->addY(10);
        margin.y = this->position->getY() + MARGIN_SIZE;
    }

    // Margen superior
    if (playerPosition->getY() < margin.y) {
        log("Jugador activo sobre el margen superior", LOG_DEBUG);
        this->position->subtractY(10);
        margin.y = this->position->getY() + MARGIN_SIZE;
    }

    //Keep the camera in bounds
    if (this->position->getX() < 0) {
        this->position->setX(0);
    }
    if (this->position->getY() < 0) {
        this->position->setY(0);
    }
    if (this->position->getX() > LEVEL_WIDTH - this->width) {
        this->position->setX(LEVEL_WIDTH - this->width);
    }
    if (this->position->getY() > LEVEL_HEIGHT - this->height) {
        this->position->setY(LEVEL_HEIGHT - this->height);
    }

    //Keep the margin in bounds
    if (margin.x < 0) {
        margin.x = 0;
    }
    if (margin.y < 0) {
        margin.y = 0;
    }
    if (margin.x > LEVEL_WIDTH - margin.w) {
        margin.x = LEVEL_WIDTH - margin.w;
    }
    if (margin.y > LEVEL_HEIGHT - margin.h) {
        margin.y = LEVEL_HEIGHT - margin.h;
    }
}

std::list<PlayerSpriteManager*> Camera::getPlayersInside(std::list<PlayerSpriteManager*>& views) {
    // TODO: Por ahora devuelve todos, falta filtrar.
    return views;
}

SDL_Rect Camera::getRectToDraw() {
    SDL_Rect renderQuad = {
        position->getX(),
        position->getY(),
        this->width,
        this->height
    };
    return renderQuad;
}

Camera::~Camera() {
    log("Camera: Liberando memoria. Borrando posicion...", LOG_INFO);
    delete(this->position);
    log("Camera: Posicion borrada. Memoria liberada.", LOG_INFO);
}
