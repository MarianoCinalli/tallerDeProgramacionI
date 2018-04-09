#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height) {
    log("Camera: Creando camara...", LOG_INFO);
    this->position = position;
    this->width = width;
    this->height = height;
    log("Camera: Camara creada.", LOG_INFO);
}

Coordinates* Camera::getRelativeCoordinates(Coordinates* absolutePosition) {
    Coordinates* relativePosition = new Coordinates(
        absolutePosition->getX() - position->getX(),
        absolutePosition->getY() - position->getY()
    );
    return relativePosition;
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
