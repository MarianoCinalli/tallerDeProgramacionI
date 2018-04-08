#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height) {
    this->position = position;
    this->width = width;
    this->height = height;
}

Coordinates* Camera::getRelativeCoordinates(Coordinates* absolutePosition) {
    Coordinates* relativePosition = new Coordinates(
        absolutePosition->getX() - position->getX(),
        absolutePosition->getY() - position->getY()
    );
    return relativePosition;
}

std::list<PlayerSpriteManager*> Camera::getPlayersInside(std::list<PlayerSpriteManager*>& views) {
    // TODO:
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
    delete(this->position);
}
