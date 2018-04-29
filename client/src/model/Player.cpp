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
    if (this->runningFast){
      return this->maxVelocity*2;
    }
    else {
      return this->maxVelocity;
    }
}

// Setea la orientacion del jugador (a donde mira).
void Player::setOrientation(int orientation) {
    // Para el caso en que el vector trayectoria sea nulo
    // deberia ser suficiente que mire en la ultima direccion
    if (orientation != PLAYER_ORIENTATION_INVALID) {
        this->orientation = orientation;
    }
}

// Si corre en dos direcciones devuelve true.
bool Player::isRunningDiagonaly() {
    return ((this->velocity->getComponentX() != 0) && (this->velocity->getComponentY() != 0));
}

void Player::toggleIsSelected() {
    this->isSelected = !this->isSelected;
    this->isReturning = true;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

void Player::accelerate(int direction) {
    // Todo: Renombrar velocity a trayectory.
    this->velocity->accelerate(direction);
    // Para que quede mirando para donde venia corriendo.
    this->setOrientation(this->velocity->getAsOrientation());
    log("Jugador: El jugador esta acelerando, direccion actual: ", this->velocity, LOG_DEBUG);
}

void Player::decelerate(int direction) {
    if (this->isRunningFast()) {
        this->velocity->decelerate(direction, this->maxVelocity);
        this->setOrientation(this->velocity->getAsOrientation());
        log("Jugador: El jugador esta frenando, velocidad actual: ", this->velocity, LOG_DEBUG);
    }
}

void Player::stopRunningInDirection(int direction) {
    if(this->isRunningFast()) {
        this->stopsRunningFast();
    }
    this->velocity->stopDirection(direction);
    log("Jugador: El Jugador deja de correr en direccion: ", direction, LOG_DEBUG);
}

void Player::stop() {
    this->velocity->stop();
    this->runningFast = false;
    log("Jugador: El jugador esta quieto, velocidad actual: ", this->velocity, LOG_DEBUG);
}


void Player::updatePosition() {
    float speed = 1;
    if (this->runningFast) {
      speed = 1.8; //TODO hardcode
    }
    int maxSpeed = this->maxVelocity;
    if (this->canMove) {
        this->position->addX(this->velocity->getComponentX()*speed*maxSpeed);
        this->position->addY(this->velocity->getComponentY()*speed*maxSpeed);
        log("Jugador: Actualizando la posicion del jugador, posicion actual: ", this->position, LOG_DEBUG);
    }

    // Si selecciona un jugador que estaba regresando lo detengo
    if (this->isSelected && this->isReturning) {
        this->isReturning = false;
        log("Player: jugador deja de volver por seleccion.", LOG_INFO);
    }

    // Detener si el jugador no seleccionado regresando llego a su posicion inicial
    if (this->isReturning) {
        int abs_delta_x = 0;
        int abs_delta_y = 0;
        abs_delta_x = abs(this->position->getX() - this->basePosition->getX());
        abs_delta_y = abs(this->position->getY() - this->basePosition->getY());
        if ((abs_delta_x < 20) && (abs_delta_y < 20)) { //TODO hardcode valores
            this->stop();
            this->isReturning = false;
            log("Player: jugador llega a la posicion.", LOG_INFO);

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
    if (newX == 0) {
        setX = 0;
    }

    if (newX < 0) {
        setX = 1;
    }

    if (newX > 0) {
        setX = -1;
    }

    if (newY == 0) {
        setY = 0;
    }

    if (newY < 0) {
        setY = 1;
    }

    if (newY > 0) {
        setY =-1;
    }

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
        !this->isRunningFast()) {
        // !this->isRunningDiagonaly()) {
            // this->velocity->accelerate(this->orientation, this->maxVelocity);
            this->runningFast = true;
            log("Jugador: El jugador corre rapido en direccion: ", this->orientation, LOG_DEBUG);
    }
}
//TODO ver que hay que apretar la tecla cuando estas corriendo, sino no tiene efecto
//TODO anda mal en diagonal

void Player::stopsRunningFast() {
    if (this->runningFast) {
        // this->velocity->decelerate(this->orientation, this->maxVelocity);
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
    if (!this->kicking) {
        this->sliding = true;
    }
    //this->velocity->accelerate(this->orientation, this->maxVelocity);
}

void Player::isAlreadySliding() {
    this->wasSliding = true;
}

void Player::stopSliding() {
    this->sliding = false;
    this->wasSliding = false;
}

void Player::copyStats(Player* copyTo) {
    copyTo->setOrientation(this->getOrientation());
    copyTo->setTrayectory(this->getVelocity());
    if (this->isRunningFast()) {
        copyTo->startsRunningFast();
    }
}

void Player::setTrayectory(Velocity* trayectory) {
    this->velocity->set(trayectory);
}
