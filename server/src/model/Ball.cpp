#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->passPower = 0;  //CTE DE 1 a 3
    this->height = 0;
    this->orientation = 0;
    this->dominated = true;
    this->velocity = new Velocity(0, 0);
    this->isInAPass = false;
    this->passDirection = 0;  //CTE DE 1 a 12 tq 360/CTE = orientacion
    this->decelerateLevel = 0;
    this->decelerateDistance = 0;
    this->timePassing = 0; // corrector para frames TODO
    this->startingPassPosition = new Coordinates(800, 500); // Esto esta perdiendo memoria?
    this->interceptable = true;
    // this->player = player;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}

Velocity* Ball::getVelocity() {
    return this->velocity;
}

Player* Ball::getPlayer() {
    return this->player;
}

void Ball::setPlayer(Player* player) {
    this->player = player;
    this->dominated = true;
    this->velocity = this->player->getVelocity();
    this->player->getsTheBall();
}

bool Ball::isDominated() {
    return this->dominated;
}

bool Ball::isInterceptable() {
    return this->interceptable;
}

void Ball::isIntercepted(Player* player) {
    this->stopRolling();
    this->interceptable = false;
    this->player->dropsTheBall();
    this->setPlayer(player);
    this->orientation = player->getOrientation();
}

void Ball::isPassed(int direction, int passPower) {
    if (this->isDominated()) {
        this->interceptable = false;
        Velocity* passDirection = new Velocity(0,0);
        passDirection->set(this->player->getVelocity());  //direction;
        this->velocity = passDirection;
        if (passDirection->isZero()) {
            int passDirection = this->player->getOrientation();
            this->velocity->accelerate(passDirection);
        }
        dominated = false;
        this->player->dropsTheBall();
        this->isInAPass = true;
        this->passPower = passPower;
        // this->decelerateDistance = passPower * 50;  //TODO: ver valores y poner ctes
        this->startingPassPosition = this->position;
    }
}

void Ball::updatePosition() {
    if (this->isDominated()) {
        this->position = this->calculateDominatedPosition();
        this->orientation = player->getOrientation();
    }
    if (this->isInAPass) {
        this->timePassing += 1;
        if (!this->interceptable && timePassing > TIME_BALL_NO_INTERCEPT) { //TODO numero harcodeado tiempo de pase
            this->interceptable = true;
        }
        this->position->addX(this->velocity->getComponentX()*this->passPower);
        this->position->addY(this->velocity->getComponentY()*this->passPower);
    }
    if ((this->isInAPass) && (!this->velocity->isZero())) {
        if (this->timePassing % BALL_DECELERATE_TIME == 0) {
            this->passPower -= 4;
            if (this->passPower< 0){
              this->timePassing = 0;
              this->stopRolling();
            }
        }
    }
    else {
        if (this->velocity->isZero()) {
            this->timePassing = 0;
            this->stopRolling();
        }
    }
}

//--------------------------PRIVATE----------------------------------

Coordinates* Ball::calculateDominatedPosition() {
    int x = this->player->getPosition()->getX(); // TODO: ver q no viole independencia
    int y = this->player->getPosition()->getY();
    switch (this->player->getOrientation()) {
        case PLAYER_ORIENTATION_RIGHT:
            x = (x + CUERPO_JUGADOR);
            break;
        case PLAYER_ORIENTATION_LEFT:
            x = (x - CUERPO_JUGADOR);
            break;
        case PLAYER_ORIENTATION_DOWN:
            y = (y + CUERPO_JUGADOR);
            break;
        case PLAYER_ORIENTATION_UP:
            y = (y - CUERPO_JUGADOR);
            break;
    }
    Coordinates* newPosition = new Coordinates(x, y);
    return newPosition;
}


void Ball::stopRolling() {
    this->velocity->stop();
    this->isInAPass = false;
    this->passPower = 0;
    this->decelerateLevel = 0;
    this->decelerateDistance = 0;
    this->height = 0;
}

/*
Convierte a YAML las propiedades necesarias para dibujar.
Esta asi, para ahorrar caracteres.
ba:
 cx: this->velocity->getComponentX()
 cy: this->velocity->getComponentY()
*/
std::string Ball::getAsYaml() {
    std::string message = "";
    message += "ba:\n";
    message += " cx: " + std::to_string(this->position->getX()) + "\n";
    message += " cy: " + std::to_string(this->position->getY()) + "\n";
    message += " st: " + std::to_string(this->velocity->isZero()) + "\n";
    return message;
}

Ball::~Ball() {
    delete(this->position);
    // delete(this->player);
    delete(this->velocity);
}
