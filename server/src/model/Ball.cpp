#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->passPower = 0;
    this->initialPassPower = 0; 
    this->height = 0;
    this->heightLevel = 0;
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

void Ball::restart(int position){
  this->dominated = false;
  this->player->setWithBall(this->dominated);
  this->stopRolling();
  this->interceptable=true;

  if (position == CENTER_START){
    this->position->setX(LEVEL_WIDTH/2);
    this->position->setY(LEVEL_HEIGHT/2);
  }
  else if (position == LEFT_START){
    this->position->setX(150);
    this->position->setY(LEVEL_HEIGHT/2);
  }
  else if (position == RIGHT_START){
    this->position->setX(LEVEL_WIDTH-150);
    this->position->setY(LEVEL_HEIGHT/2);
  }
}

void Ball::setPlayer(Player* player) {
    this->player = player;
    this->dominated = true;
    this->velocity = this->player->getVelocity();
    this->player->setWithBall(this->dominated);
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
    log("Ball: Pelota interceptada por jugador.", LOG_DEBUG);
}

void Ball::isPassed(int direction, int passPower, bool highPass) {
    if (this->isDominated()) {
        this->interceptable = false;
        Velocity* passDirection = new Velocity(0,0);
        passDirection->set(this->player->getVelocity());  //direction;
        this->velocity = passDirection;
        log(this->velocity->toString(),LOG_DEBUG);
        // log()
        if (passDirection->isZero()) {
            log("Ball: jugador tiene velocidad cero", LOG_DEBUG);
            int passOrientation = this->player->getOrientation();
            this->velocity->accelerate(passOrientation);
        }
        dominated = false;
        this->player->setWithBall(this->dominated);
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
        if(this->isInAHighPass && !this->velocity->isZero()) {
            this->calculateHeight();
            if(this->height > BALL_DECELERATE_CONST){
                this->interceptable = false;
            }
            log("Ball: altura del balon: ", this->height, LOG_DEBUG);
            log("Ball: poder de pase: ", this->passPower, LOG_DEBUG);
        }
        if (!this->interceptable && timePassing > TIME_BALL_NO_INTERCEPT) { //TODO numero harcodeado tiempo de pase
            this->interceptable = true;
        }
        if(this->position->addX(this->velocity->getComponentX()*this->passPower)<0){
          this->velocity->scaleY(-1);
        }
        if(this->position->addY(this->velocity->getComponentY()*this->passPower)<0){
          this->velocity->scaleY(-1);
        }
    }
    if ((this->isInAPass) && (!this->velocity->isZero())) {
        if (this->timePassing % BALL_DECELERATE_TIME == 0) {
            this->passPower -= BALL_DECELERATE_CONST;
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

void Ball::calculateHeight() {
    float initialPassPower = (this->initialPassPower * 6) / 8;
    float passPower = (this->passPower * 6) / 8;
    if ((initialPassPower * 0.7) < passPower) {
        this->height = (this->initialPassPower - this->passPower);
    }
    else{
        if (this->height > BALL_DECELERATE_CONST) {
            if ((initialPassPower * 0.6) < passPower && decelerateLevel == 0) {
                this->height = this->height - BALL_DECELERATE_CONST;
                this->decelerateLevel = 1;
            }
            else {
                if((initialPassPower * 0.45) < passPower && decelerateLevel == 1) {
                this->height = this->height - BALL_DECELERATE_CONST;
                this->decelerateLevel = 2;
                }
                else {
                    if((initialPassPower * 0.3) < passPower && decelerateLevel == 2) {
                    this->height = this->height - BALL_DECELERATE_CONST;
                    this->decelerateLevel = 3;
                    }
                    else {
                        if (decelerateLevel == 3) {
                        this->height = 0;
                        decelerateLevel = 0;
                        }
                    }
                }
            }
        }
        else{
            this->height = 0;
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
 st: this->velocity->isZero()
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
