#include "Ball.h"
#include <math.h>

Ball::Ball(Coordinates* position, Player* player) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->passPower = 0;  //CTE DE 1 a 3
    this->height = 0;
    this->orientation = 0;
    this->isDominated = true;
    this->velocity = new Velocity(0, 0);
    this->isInAPass = false;
    this->passDirection = 0;  //CTE DE 1 a 12 tq 360/CTE = orientacion
    this->decelerateLevel = 0;
    this->player = player;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}

Velocity* Ball::getVelocity() {
    return this->velocity;
}

bool Ball::isDominated() {
	return this->isDominated;
}

void Ball::isIntercepted(Player* player) {
	this->stopRolling();
	this->player = player;
	this->isDominated = true;
	this->orientation = player->getOrientation();
}

void Ball::isPassed(int direction, int passPower) {
	if (this->isDominated()) {
		passDirection = direction;
		isDominated = false;
		this->velocity->accelerate(direction, passPower);
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
	if((this->isInAPass) && (!this->velocity->isZero())) {
		float passDistance = 0;
		int abs_delta_x = 0;
        int abs_delta_y = 0;
        abs_delta_x = abs(this->position->getX() - this->startingPassPosition->getX());
        abs_delta_y = abs(this->position->getY() - this->startingPassPosition->getY());
		passDistance = (sqrt((abs_delta_x * abs_delta_x) + (abs_delta_y * abs_delta_y)));
		if (passDistance > decelerateDistance) {
			this->progresiveDecelerate(passDistance);
		}
	}
	else {
		if (this->velocity->isZero()) {
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
			x = (x + CUERPO_JUGAROR);
			break;
		case PLAYER_ORIENTATION_LEFT:
			x = (x - CUERPO_JUGAROR);
			break;
		case PLAYER_ORIENTATION_DOWN:
			y = (y + CUERPO_JUGAROR);
			break;
		case PLAYER_ORIENTATION_UP:
			y = (y - CUERPO_JUGAROR);
			break;
	}
	Coordinates* newPosition = new Coordinates(x, y);
	return newPosition;
}

void Ball::progressiveDecelerate(float passDistance) {
	if ((passDistance > decelerateDistance) && decelerateLevel = 0) {
		float amount = (this->velocity->totalVelocity() / 4); 
		this->velocity->decelerate(this->direction, amount);
		this->decelerateLevel = 1;
	}
	else {
		if ((passDistance > (1.5*decelerateDistance)) && (decelerateLevel = 1)) {
			float amount = (this->velocity->totalVelocity() / 4);
			this->velocity->decelerate(this->direction, amount);
			this->decelerateLevel = 2; 	
		}
		else {
			if ((passDistance > (1.75*decelerateDistance)) && (decelerateLevel = 2)) {
			float amount = (this->velocity->totalVelocity() / 4);
			this->velocity->decelerate(this->direction, amount);
			this->decelerateLevel = 3; 					
			}
			else { 
				if ((passDistance > (2*decelerateDistance)) && (decelerateLevel = 3)) {
				float amount = (this->velocity->totalVelocity() / 4);
				this->velocity->decelerate(this->direction, amount);
				this->decelerateLevel = 4; 					
				}
			}
		}
	}
}

void Ball::stopRolling() {
	this->velocity->stop();
	this->isInAPass = false;
	this->passPower = 0;
	this->decelerateDistance = 0;
	this->decelerateLevel = 0;
	this->height = 0;
}

Ball::~Ball() {
    delete(this->position);
    delete(this->player);
    delete(this->velocity);
}
