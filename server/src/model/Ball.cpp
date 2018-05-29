#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->passPower = 0;
    this->initialPassPower = 0; 
    this->height = 0;
    this->orientation = 0;
    this->dominated = true;
    this->velocity = new Velocity(0, 0);
    this->isInAPass = false;
    this->isInAHighPass = false;
    this->passDirection = 0;  
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

int Ball::getHeight() {
    return this->height;
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
    this->player->isWithBall(this->dominated);
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
    this->setPlayer(player);
    this->orientation = player->getOrientation();
}

void Ball::isPassed(int direction, int passPower, bool highPass) {
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
        this->player->isWithBall(this->dominated);
        this->isInAPass = true;
        if(highPass) {
            this->isInAHighPass = true;
        }
        this->passPower = passPower;
        this->initialPassPower = passPower;
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
            this->passPower -= 3;
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
    if(this->isInAHighPass) {
        this->calculateHeight();
    }
}

//--------------------------PRIVATE----------------------------------

void Ball::calculateHeight() {
    if (!this->velocity->isZero()) {
        if ((initialPassPower * 0.5) < passPower) {
            if ((initialPassPower * 0.75) < passPower) {
                if ((initialPassPower * 0.825) < passPower) {
                    if ((initialPassPower * 0.9) < passPower) {
                        if (heightLevel == 0) {
                            this->height++;
                            this->heightLevel = 1;
                        }
                    }
                    else {
                        if (heightLevel == 1) {
                            this->height++;
                            this->heightLevel = 2;
                        }
                    }
                }
                else{
                    if (heightLevel == 2) {
                        this->height++;
                        this->heightLevel = 3;
                    }
                }
            }
            else{
                if (heightLevel == 3) {
                    this->height++;
                    this->heightLevel == 4;
                }
            }
        }
        else{     //empieza a bajar
            if ((initialPassPower * 0.1) < passPower) {
                if ((initialPassPower * 0.125) < passPower) {
                    if ((initialPassPower * 0.25) < passPower) {
                        if (heightLevel == 4) {
                            this->height--;
                            this->heightLevel = 5;
                        }
                    }
                    else {
                        if (heightLevel == 5) {
                            this->height--;
                            this->heightLevel = 6;
                        }
                    }
                }
                else{
                    if (heightLevel == 6) {
                        this->height--;
                        this->heightLevel = 7;
                    }
                }
            }
            else{
                if (heightLevel == 7) {
                    this->height--;
                    this->heightLevel == 8;
                }
            }
        }
    }
}

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
    this->isInAHighPass = false;
    this->passPower = 0;
    this->decelerateLevel = 0;
    this->decelerateDistance = 0;
    this->height = 0;
    this->heightLevel = 0;
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
    message += " hg: " + std::to_string(this->height) + "\n";
    message += " or: " + std::to_string(this->orientation) + "\n";
    return message;
}

Ball::~Ball() {
    delete(this->position);
    // delete(this->player);
    delete(this->velocity);
}
