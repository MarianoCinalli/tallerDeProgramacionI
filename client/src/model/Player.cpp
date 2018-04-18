#include "model/Player.h"

Player::Player(int orientation, Coordinates* position) {
    log("Jugador: Creando jugador...", LOG_INFO);
    this->orientation = orientation;
    this->position = position;
    this->basePosition = new Coordinates(800, 500);
    this->maxVelocity = NORMAL_VELOCITY; // TODO: Probar si va muy rapido.
    this->velocity = new Velocity(0, 0); // Empieza quieto.
    this->sliding = false;
    this->wasSliding = false;   //Deberia estar en PlayerSpriteManager
    this->kicking = false;
    this->wasKicking = false;
    this->canMove = true;
    this->isSelected = false;
    this->isReturning = false;
    this->runningFast = false;
    log("Jugador: Jugador creado.", LOG_INFO);
}

Coordinates* Player::getPosition() {
    return this->position;
}

int Player::getOrientation() {
    return this->orientation;
}

Velocity* Player::getVelocity() {
    return this->velocity;
}

int Player::getCurrentSpeed() {
    return NORMAL_VELOCITY;
}

void Player::setOrientation(int orientation) {
    if (this->isRunningDiagonaly()) {
        if (this->velocity->getComponentX() > 0) {
            this->orientation = PLAYER_ORIENTATION_RIGHT;
        } else {
            this->orientation = PLAYER_ORIENTATION_LEFT;
        }
    } else {
        this->orientation = orientation;
    }
}

// Si corre en dos direcciones devuelve true.
bool Player::isRunningDiagonaly() {
    return ((this->velocity->getComponentX() != 0) && (this->velocity->getComponentY() != 0));
}

void Player::toggleIsSelected() {
    this->isSelected = !this->isSelected;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

void Player::accelerate(int direction) {
    this->velocity->accelerate(direction, this->maxVelocity);
    this->setOrientation(direction); // Para que quede mirando para donde venia corriendo.
    log("Jugador: El jugador esta acelerando, velocidad actual: ", this->velocity, LOG_DEBUG);
}

void Player::decelerate(int direction) {
    if (this->isRunningFast()) {
        this->velocity->decelerate(direction, this->maxVelocity);
        /*if (this->velocity->getComponentY() != 0) {
            if (this->velocity->getComponentY() > 0) {
                this->orientation = PLAYER_ORIENTATION_DOWN;
            } else {
                this->orientation = PLAYER_ORIENTATION_UP;
            }
        } else {
            this->orientation = orientation;
        }*/
        log("Jugador: El jugador esta frenando, velocidad actual: ", this->velocity, LOG_DEBUG);
    }
}

void Player::stopRunningInDirection(int direction) {
    if(this->isRunningFast()) {
        this->stopsRunningFast();
    }
    this->velocity->stopRunningIn(direction);
    log("Jugador: El Jugador deja de correr en direccion: ", direction, LOG_DEBUG);
}

void Player::stop(int direction) {
    this->velocity->stop();
    this->orientation = direction;
    log("Jugador: El jugador esta quieto, velocidad actual: ", this->velocity, LOG_DEBUG);
}


void Player::updatePosition() {
    if (this->canMove) {
        this->position->addX(this->velocity->getComponentX());
        this->position->addY(this->velocity->getComponentY());
        log("Jugador: Actualizando la posicion del jugador, posicion actual: ", this->position, LOG_DEBUG);
    }

    // Si selecciona un jugador que estaba regresando lo detengo
    if (this->isSelected && this->isReturning) {
        this->isReturning = false;
        this->stop(this->orientation);
    }

    // Detener si el jugador no seleccionado regresando llego a su posicion inicial
    int abs_delta_x = 0;
    int abs_delta_y = 0;
    if (!this->isSelected) {
        abs_delta_x = abs(this->position->getX() - this->basePosition->getX());
        abs_delta_y = abs(this->position->getY() - this->basePosition->getY());
        if ((abs_delta_x < 15) && (abs_delta_y < 15)) {
            this->stop(this->orientation);
        } else {
            this->returnToBasePosition();
        }
    }
}

void Player::setPosition(Coordinates pos) {
    this->position->setCoord(pos);
}

void Player::setBasePosition(Coordinates pos) {
    this->basePosition->setCoord(pos);
}

void Player::returnToBasePosition() {
    log("Jugador: Volviendo a su posicion base ", LOG_DEBUG);
    this->isReturning = true;

    // Calcular en que direccion debe regresar
    int newX = this->position->getX() - this->basePosition->getX();
    int newY = this->position->getY() - this->basePosition->getY();

    //TODO: Normalizar el vector al factor maxVelocity para que corra por el camino mas corto
    int setX = newX;
    int setY = newY;
    if (newX == 0)
        setX = 0;
    
    if (newX < 0)
        setX = this->maxVelocity;
    
    if (newX > 0) 
        setX = this->maxVelocity * -1;
    
    if (newY == 0) 
        setY = 0;
    
    if (newY < 0) 
        setY = this->maxVelocity;
    
    if (newY > 0) 
        setY = this->maxVelocity * -1;

    this->velocity->setComponentX(setX);
    this->velocity->setComponentY(setY);
}

Player::~Player() {
    delete(this->position);
    delete(this->basePosition);
    delete(this->velocity);
}

void Player::startsRunningFast() {
    if (!this->sliding && 
        !this->kicking && 
        !this->velocity->isZero() && 
        !this->isRunningFast() &&
        !this->isRunningDiagonaly()) {
            this->velocity->accelerate(this->orientation, this->maxVelocity);
            this->runningFast = true;
            log("Jugador: El jugador corre rapido en direccion: ", this->orientation, LOG_DEBUG);
    }
}
//TODO ver que hay que apretar la tecla cuando estas corriendo, sino no tiene efecto
//TODO anda mal en diagonal

void Player::stopsRunningFast() {
    if (this->runningFast) {
        this->velocity->decelerate(this->orientation, this->maxVelocity);
        this->runningFast = false;
        //log("Jugador: El jugador deja de correr rapido", LOG_DEBUG);
    }
}

bool Player::isRunningFast() {
    return this->runningFast;
}

//SLIDE AND KICK FUNCTIONS

void Player::startsKicking() {
    if (!this->sliding) {
        this->kicking = true;
        this->canMove = false;
    }
}

void Player::stopKicking() {
    this->kicking = false;
    this->wasKicking = false;
    this->canMove = true;
}

bool Player::isKicking() {
    return this->kicking;
}

bool Player::wasKickingYet() {
    return this->wasKicking;
}

void Player::isAlreadyKicking() {
    this->wasKicking = true;
}
bool Player::isSliding() {
    return this->sliding;
}

bool Player::wasSlidingYet() {
    return this->wasSliding;
}

void Player::startsSliding() {
    if (!this->kicking)
        this->sliding = true;
    //this->velocity->accelerate(this->orientation, this->maxVelocity);
}

void Player::isAlreadySliding() {
    this->wasSliding = true;
}

void Player::stopSliding() {
    this->sliding = false;
    this->wasSliding = false;
}
