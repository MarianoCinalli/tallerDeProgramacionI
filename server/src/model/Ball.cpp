#include "model/Ball.h"

Ball::Ball(Coordinates* position, Player* player) {
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
    this->startingPassPosition = new Coordinates (800, 500);
    this->interceptable = true;
    this->player = player;
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
  this->dominated= true;
}

bool Ball::isDominated() {
	return this->dominated;
}

bool Ball::isInterceptable(){
  return this->interceptable;
}

void Ball::isIntercepted(Player* player) {
	this->stopRolling();
  this->interceptable = false;
	this->player = player;
	this->dominated = true;
	this->orientation = player->getOrientation();
}

void Ball::isPassed(int direction, int passPower) {
	if (this->isDominated()) {
    this->interceptable =false;
		Velocity* passDirection = this->player->getVelocity();  //direction;
    this->velocity->set(passDirection);
    if (passDirection->isZero()) {
      int passDirection = this->player->getOrientation();
      this->velocity->accelerate(passDirection);
    }
		dominated = false;
		this->isInAPass = true;
		this->passPower = passPower;
		this->decelerateDistance = passPower * 50;  //TODO: ver valores y poner ctes
		this->startingPassPosition = this->position;
	}
}

void Ball::updatePosition() {
	//if (((!this->isDominated) && this->velocity->totalVelocity()) < VELOCIDAD_MINIMA_BALON) {
	//	this->stopRolling();
	//}

	if(this->isDominated()) {
		this->position = this->calculateDominatedPosition();
		this->orientation = player->getOrientation();
	}
  if(this->isInAPass){
    this->timePassing += 1;
    if (!this->interceptable && timePassing > TIME_BALL_NO_INTERCEPT){ //TODO numero harcodeado tiempo de pase
      this->interceptable = true;
    }
    this->position->addX(this->velocity->getComponentX()*this->passPower);
    this->position->addY(this->velocity->getComponentY()*this->passPower);
  }
  if((this->isInAPass) && (!this->velocity->isZero())) {
    if(this->timePassing % BALL_DECELERATE_TIME == 0){
      this->velocity->decelerate(1);
    }
  }
	// if((this->isInAPass) && (!this->velocity->isZero())) {
	// 	float passDistance = 0;
	// 	int abs_delta_x = 0;
  //       int abs_delta_y = 0;
  //       abs_delta_x = abs(this->position->getX() - this->startingPassPosition->getX());
  //       abs_delta_y = abs(this->position->getY() - this->startingPassPosition->getY());
	// 	passDistance = (sqrt((abs_delta_x * abs_delta_x) + (abs_delta_y * abs_delta_y)));
	// 	if (passDistance > decelerateDistance) {
	// 		this->progressiveDecelerate(passDistance);
	// 	}
	// }
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
	switch(this->player->getOrientation()) {
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
//
// void Ball::progressiveDecelerate(float passDistance) {
// 	if ((passDistance > decelerateDistance) && (this->decelerateLevel) == 0) {
// 		float amount = (this->velocity->totalVelocity() / 4);
// 		this->velocity->decelerate(this->orientation, amount);
// 		this->decelerateLevel = 1;
// 	}
// 	else {
// 		if ((passDistance > (1.5*decelerateDistance)) && (decelerateLevel == 1)) {
// 			float amount = (this->velocity->totalVelocity() / 4);
// 			this->velocity->decelerate(this->orientation, amount);
// 			this->decelerateLevel = 2;
// 		}
// 		else {
// 			if ((passDistance > (1.75*decelerateDistance)) && (decelerateLevel == 2)) {
// 			float amount = (this->velocity->totalVelocity() / 4);
// 			this->velocity->decelerate(this->orientation, amount);
// 			this->decelerateLevel = 3;
// 			}
// 			else {
// 				if ((passDistance > (2*decelerateDistance)) && (decelerateLevel == 3)) {
// 				float amount = (this->velocity->totalVelocity() / 4);
// 				this->velocity->decelerate(this->orientation, amount);
// 				this->decelerateLevel = 4;
// 				}
// 			}
// 		}
// 	}
// }

void Ball::stopRolling() {
	this->velocity->stop();
	this->isInAPass = false;
	this->passPower = 0;
	this->decelerateLevel = 0;
	this->decelerateDistance = 0;
	this->height = 0;
}

Ball::~Ball() {
    delete(this->position);
    // delete(this->player);
    delete(this->velocity);
}
