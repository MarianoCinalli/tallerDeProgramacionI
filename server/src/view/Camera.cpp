#include "view/Camera.h"

Camera::Camera(Coordinates* position, int width, int height, int margen) {
    log("Camera: Creando camara...", LOG_INFO);
    this->position = position;
    this->width = width;
    this->height = height;
    this->margin_size = margen;
    //The margin area
    this->margin = { position->getX() + margin_size, position->getY() + margin_size , width - margin_size*2 , height - margin_size*2};
    log("Camera: Camara creada.", LOG_INFO);
}

Coordinates Camera::getCenter(){
  int x = this->position->getX() + this->width/2;
  int y = this->position->getY() + this->height/2;
  return Coordinates(x, y);
}

std::string Camera::getAsYaml() {
    std::string message = "";
    Coordinates center = getCenter();
    message += "cam:\n";
    message += " cx: " + std::to_string(center.getX()) + "\n";
    message += " cy: " + std::to_string(center.getY()) + "\n";
    return message;
}

void Camera::calculateNewPosition(Coordinates* objPosition) {
    // Margen derecho
    int x = objPosition->getX();
    int y = objPosition->getY();
    int difference;
    // Margen derecho
    difference = x + SPRITE_SIZE - (margin.x + margin.w);
    if (difference > 0) {
        log("Camera: Jugador activo a la derecha del margen derecho", LOG_SPAM);
        this->position->addX(difference);
        margin.x = this->position->getX() + margin_size ;
    }
    // Margen izquierdo
    difference = x - margin.x;
    if (difference < 0) {
        log("Camera: Jugador activo a la izquierda del margen izquierdo", LOG_SPAM);
        this->position->addX(difference);
        margin.x = this->position->getX() + margin_size ;
    }
    // Margen inferior
    difference = (y + SPRITE_SIZE) - (margin.y + margin.h);
    if (difference > 0) {
        log("Camera: Jugador activo debajo del margen inferior", LOG_SPAM);
        this->position->addY(difference);
        margin.y = this->position->getY() + margin_size;
    }
    // Margen superior
    difference = y - margin.y;
    if (difference < 0) {
        log("Camera: Jugador activo sobre el margen superior", LOG_SPAM);
        this->position->addY(difference);
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
}


bool Camera::isInsideMargin(Player* p, SDL_Rect* margin, int outerMargin) {
    int playerX = p->getPosition()->getX();
    int playerY = p->getPosition()->getY();
    int offset = 80;
    if (!outerMargin) {
        if ((playerX > margin->x) &&
                (playerX < margin->x + margin->w) &&
                (playerY > margin->y) &&
                (playerY < margin->y + margin->h)) {
            return true;
        } else {
            return false;
        }
    } else {
        if ((playerX > this->position->getX() - offset) &&
                (playerX < this->position->getX() + this->width + offset) &&
                (playerY > this->position->getY() - offset) &&
                (playerY < this->position->getY() + this->height + offset)) {
            return true;
        } else {
            return false;
        }
    }
}

std::list<Player*> Camera::getPlayersInsideMargin(std::list<Player*> players, int outerMargin) {
    // Solo los que estan dentro de los margenes
    std::list<Player*> resultado;
    for (Player* p : players) {
        if (isInsideMargin(p, &margin, outerMargin)) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

Camera::~Camera() {
    log("Camera: Liberando memoria. Borrando posicion...", LOG_INFO);
    delete(this->position);
    log("Camera: Posicion borrada. Memoria liberada.", LOG_INFO);
}
