#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height) {
    log("Camera: Creando camara...", LOG_INFO);
    this->position = position;
    this->width = width;
    this->height = height;
    log("Camera: Camara creada.", LOG_INFO);
}

void Camera::parseYaml(YAML::Node node){
  if (node["cx"]){
    this->position->setX(node["cx"].as<int>());
  }
  if (node["cy"]){
    this->position->setY(node["cy"].as<int>());
  }
}

Coordinates* Camera::getRelativeCoordinates(Coordinates* absolutePosition) {
    Coordinates* relativePosition = new Coordinates(
        absolutePosition->getX() - position->getX(),
        absolutePosition->getY() - position->getY()
    );
    return relativePosition;
}


std::list<PlayerSpriteManager*> Camera::getPlayersInside(std::list<PlayerSpriteManager*>& views) {
    // Solo los que estan dentro de la camara
    std::list<PlayerSpriteManager*> resultado;
    int playerX = 0;
    int playerY = 0;
    for (PlayerSpriteManager* p : views){
        playerX = p->getPlayerCoordinates()->getX();
        playerY = p->getPlayerCoordinates()->getY();
        if ( (playerX > 0 - SPRITE_SIZE) &&
             (playerX < LEVEL_WIDTH) &&
             (playerY > 0 - SPRITE_SIZE) &&
             (playerY < LEVEL_HEIGHT) ) {
            // Esta dentro de los limites
            resultado.push_back(p);
        }
    }
    return resultado;
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
