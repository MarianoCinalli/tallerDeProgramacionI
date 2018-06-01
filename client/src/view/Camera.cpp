#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height, int margen) {
    log("Camera: Creando camara...", LOG_INFO);
    this->position = position;
    this->width = width;
    this->height = height;
    this->margin_size = margen;
    //The margin area
    this->margin = { position->getX() + margin_size, position->getY() + margin_size, width - margin_size * 2, height - margin_size * 2 };
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

void Camera::calculateNewPostion(Coordinates* centerPosition, int scrollSpeed) {
    // Margen derecho
    if ((centerPosition->getX() + SPRITE_SIZE) > (margin.x + margin.w)) {
        log("Jugador activo a la derecha del margen derecho", LOG_DEBUG);
        this->position->addX(scrollSpeed);
        margin.x = this->position->getX() + margin_size;
    }

    // Margen izquierdo
    if (centerPosition->getX() < margin.x) {
        log("Jugador activo a la izquierda del margen izquierdo", LOG_DEBUG);
        this->position->subtractX(scrollSpeed);
        margin.x = this->position->getX() + margin_size;
    }

    // Margen inferior
    if ((centerPosition->getY() + SPRITE_SIZE) > (margin.y + margin.h)) {
        log("Jugador activo debajo del margen inferior", LOG_DEBUG);
        this->position->addY(scrollSpeed);
        margin.y = this->position->getY() + margin_size;
    }

    // Margen superior
    if (centerPosition->getY() < margin.y) {
        log("Jugador activo sobre el margen superior", LOG_DEBUG);
        this->position->subtractY(scrollSpeed);
        margin.y = this->position->getY() + margin_size;
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

bool isInsideMargin(Player* p,SDL_Rect* margin){
  int playerX = p->getPosition()->getX();
  int playerY = p->getPosition()->getY();
  if ((playerX > margin->x) &&
       (playerX < margin->x + margin->w) &&
       (playerY > margin->y) &&
       (playerY < margin->y + margin->h )) {
         return true;
       }
  else {
    return false;
  }
}

std::list<Player*> Camera::getPlayersInsideMargin(std::list<Player*> players) {
    // Solo los que estan dentro de los margenes
    std::list<Player*> resultado;
    for (Player* p : players){
        if (isInsideMargin(p,&margin)){
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
