#include "model/Player.h"

int Player::ID = 0;

const float  NORMAL_VELOCITY = 1.6;
const float FAST_SPEED_COEF = 1.6;

Player::Player(Coordinates* position, int team, float speed, float sprint) {
    log("Player: Creando jugador...", LOG_INFO);
    this->id = ++ID;
    if (team == 0) {
        this->orientation =  PLAYER_ORIENTATION_RIGHT;
    } else {
        this->orientation = PLAYER_ORIENTATION_LEFT;
    }
    this->position = position;
    this->team = team;
    this->basePosition = new Coordinates(800, 500);
    this->maxVelocity = speed; // TODO: Probar si va muy rapido.
    this->sprintVelocity = sprint;
    this->velocity = new Velocity(0, 0); // Empieza quieto.
    this->sliding = false;
    this->slided = false; //Deberia estar en PlayerSpriteManager
    this->kicking = false;
    this->kicked = false;
    this->canMove = true;
    this->kickPower = 1;    //inicializacion default?
    this->highPass = false;
    this->isSelected = false;
    this->isReturning = false;
    this->runningFast = false;
    this->kickCount = 0;
    this->slideCount = 0;
    this->withBall = false;
    this->userName = "NONE";
    this->stealCoef = DEFENSE_STEAL_COEF;
    log("Player: Jugador creado.", LOG_INFO);
}

Coordinates* Player::getPosition() {
    return this->position;
}

int Player::getTeam() {
    return this->team;
}

int Player::getId() {
    return this->id;
}
int Player::getStealCoef() {
    return this->stealCoef;
}

void Player::setWithBall(bool dominated) {
    this->withBall = dominated;

}

bool Player::isWithBall() {
    return this->withBall;
}

bool Player::isAHighPass() {
    return this->highPass;
}

bool Player::isThisPlayer(Player* player) {
    return (this->position->isThisPosition(player->getPosition()));
}

int Player::getOrientation() {
    if (!this->velocity->isZero()) {
        this->orientation = this->velocity->getAsOrientation();
    }
    return this->orientation;
}

int Player::getKickPower() {
    return this->kickPower;
}

Velocity* Player::getVelocity() {
    return this->velocity;
}

int Player::getCurrentSpeed() {
    if (this->runningFast) {
        return this->maxVelocity * FAST_SPEED_COEF; //Hardcode
    } else {
        return this->maxVelocity;
    }
}

void Player::setMovement(PlayerMovement* playerMovement) {
    this->playerMovement = playerMovement;
}

void Player::setCanMove(bool move) {
    this->canMove = move;
}

void Player::setFieldPosition(int formation) {
    int number = this->id;
    if (number > 7) {
        number -= 7;
    }
    int defense = formation / 1 % 10;
    int midfield = formation / 10 % 10;
    if (number == 1) {
        this->fieldPosition = KEEPER_POSITION;
        this->stealCoef = KEEPER_STEAL_COEF;
    } else if (number <= (1 + defense)) {
        this->fieldPosition = DEFENSE_POSITION;
        this->stealCoef = DEFENSE_STEAL_COEF;
    } else if (number <= (1 + defense + midfield)) {
        this->fieldPosition = MIDFIELD_POSITION;
        this->stealCoef = MIDFIELD_STEAL_COEF;
    } else {
        this->fieldPosition = STRIKER_POSITION;
        this->stealCoef = STRIKER_STEAL_COEF;
    }
    log("Player: posicion en cancha: ", this->fieldPosition, LOG_DEBUG);
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

void Player::toggleIsSelected(std::string name) {
    if (this->isSelected) {
        this->runningFast = false;
        this->userName = "NONE";
    } else {
        this->userName = name;
    }
    this->isSelected = !this->isSelected;
    this->isReturning = true;
}

std::string Player::getUsername() {
    return this->userName;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

void Player::accelerate(int direction) {
    // Todo: Renombrar velocity a trayectory.
    this->velocity->accelerate(direction);
    // Para que quede mirando para donde venia corriendo.
    this->setOrientation(this->velocity->getAsOrientation());
    log("Player: El jugador esta acelerando, direccion actual: ", this->velocity, LOG_SPAM);
}

void Player::stopRunningInDirection(int direction) {
    if (this->isRunningFast()) {
        this->stopsRunningFast();
    }
    this->velocity->stopDirection(direction);
    log("Player: El Jugador deja de correr en direccion: ", direction, LOG_DEBUG);
}

void Player::stop() {
    this->velocity->stop();
    // this->runningFast = false;
    log("Player: El jugador esta quieto, velocidad actual: ", this->velocity, LOG_SPAM);
}


void Player::updateState(Coordinates* ballPosition, bool isAttacking) {
    this->updatePosition(ballPosition, isAttacking); //follows this position
    this->updateKicking();
    this->updateSliding(ballPosition);
}
//
// int frameDiference(int current, int last){
//   int frameDelta = current -last;
//   if (frameDelta < 0){
//     frameDelta += FRAME_WINDOW;
//   }
//   return frameDelta;
// }

void Player::updateKicking() {
    if (this->kicking) {
        kickCount++;
    }
    if (kickCount > KICK_COUNT) {
        this->stopKicking();
        kickCount = 0;
    }

}

const int DISTANCE_TO_STEAL = 15;

void Player::updateSliding(Coordinates* ballPosition) {
    int number = this->id;
    if (number > 7) {
        number -= 7;
    }
    if (number == 1) {
        if (this->position->distanceTo(ballPosition) < DISTANCE_TO_STEAL) {
            this->startsSliding();
        }
    }
    if (this->sliding) {
        slideCount++;
    }
    if (slideCount > SLIDE_COUNT) {
        this->stopSliding();
        slideCount = 0;
    }

}

void Player::updatePosition(Coordinates* positionToFollow, bool isAttacking) {
    float speed = 1;
    if (this->runningFast) {
        speed = this->sprintVelocity; //TODO hardcode
    }
    if (!this->isSelected) {
        if (this->playerMovement->isInsideArea(this->position, this->id)) {
            if (this->isGoalkeeper()) {
                // El arquero solo se mueve en Y.
                this->changeVelocityTo(positionToFollow, false, true);
            } else {
                Coordinates* coordinatesToFollow = this->playerMovement->getCoordinatesToFollow(
                    positionToFollow,
                    this->position,
                    this->basePosition,
                    this->id,
                    isAttacking
                );
                this->changeVelocityTo(coordinatesToFollow, false, false);
                this->playerMovement->cleanVelocity(this->velocity, this->position, this->id);
                delete(coordinatesToFollow);
            }
        } else {
            // Si no vuelve a la posicion original.
            this->changeVelocityTo(this->basePosition, false, false);
        }
    }
    if (this->canMove) {
        float amountX = this->velocity->getComponentX() * speed * this->maxVelocity;
        float amountY = this->velocity->getComponentY() * speed * this->maxVelocity;
        this->position->addY(amountY);
        this->position->addX(amountX);
    }
}

void Player::changeVelocityTo(Coordinates* positionToFollow, bool onlyX, bool onlyY) {
    int deltaX = positionToFollow->getX() - this->position->getX();
    int deltaY = positionToFollow->getY() - this->position->getY();
    // Normalizacion.
    this->stop();
    if (onlyX) {
        deltaY = 0;
    } else if (onlyY) {
        deltaX = 0;
    }
    this->velocity->setComponentX(deltaX);
    this->velocity->setComponentY(deltaY);
    this->velocity->normalize();
    if (!this->velocity->isZero()) {
        this->orientation = this->velocity->getAsOrientation();
    }
}

bool Player::isGoalkeeper() {
    return this->id == 1 || this->id == 8;
}

void Player::setPosition(Coordinates pos) {
    this->position->set(pos);
}

void Player::setBasePosition(Coordinates pos) {
    this->basePosition->set(pos);
}

void Player::setPosition(Coordinates* pos) {
    this->position->set(*pos);
}

void Player::setBasePosition(Coordinates* pos) {
    this->basePosition->set(*pos);
}

Player::~Player() {
    delete(this->position);
    delete(this->basePosition);
    delete(this->velocity);
}

void Player::startsRunningFast() {
    if (!this->sliding && !this->kicking && !this->isRunningFast()) {
        this->runningFast = true;
        log("Player: El jugador corre rapido en direccion: ", this->orientation, LOG_DEBUG);
    }
}
//TODO ver que hay que apretar la tecla cuando estas corriendo, sino no tiene efecto
//TODO anda mal en diagonal

void Player::stopsRunningFast() {
    if (this->runningFast) {
        this->runningFast = false;
        //log("Player: El jugador deja de correr rapido", LOG_DEBUG);
    }
}

bool Player::isRunningFast() {
    return this->runningFast;
}

//SLIDE AND KICK FUNCTIONS

void Player::startsKicking(int power, bool highPass) {
    if (!this->sliding) {
        this->kickPower = power;
        this->highPass = highPass;
        this->kicking = true;
        this->kicked = false;
        this->canMove = false;
    }
}

void Player::stopKicking() {
    this->kicking = false;
    this->kicked = false;
    this->canMove = true;
}

bool Player::isKicking() {
    return this->kicking;
}


bool Player::hasKicked() {
    return this->kicked;
}

void Player::setKicked(bool k) {
    this->kicked = k;
}

bool Player::isSliding() {
    return this->sliding;
}

bool Player::slidedYet() {
    return this->slided;
}

void Player::startsSliding() {
    if (!this->kicking && !this->withBall) {
        this->sliding = true;
        this->slided = false;
    }
}

void Player::setSlided(bool s) {
    this->slided = s;
}

void Player::stopSliding() {
    this->sliding = false;
    this->slided = false;
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
bool Player::isStill() {
    if (this->velocity->isZero()) {
        return true;
    } else {
        return false;
    }
}


/*
   Convierte a YAML las propiedades necesarias para dibujar.
   Esta asi, para ahorrar caracteres.
   this->id:
   te: this->team
   cx: this->position->getX()
   cy: this->position->getY()
   se: this->isSelected
   ki: this->kicking
   sl: this->sliding
   ru: this->runningFast
   st: isStill()
   or: this->orientation
   nm: name
 */
std::string Player::getAsYaml() {
    std::string message = "";
    message += std::to_string(this->id) + ":\n";
    message += " te: " + std::to_string(this->team) + "\n";
    message += " cx: " + std::to_string((int)this->position->getX()) + "\n";
    message += " cy: " + std::to_string((int)this->position->getY()) + "\n";
    message += " se: " + std::to_string(this->isSelected) + "\n";
    message += " ki: " + std::to_string(this->kicking) + "\n";
    message += " sl: " + std::to_string(this->sliding) + "\n";
    message += " ru: " + std::to_string(this->runningFast) + "\n";
    message += " or: " + std::to_string(this->orientation) + "\n";
    message += " nm: " + this->userName + "\n";
    message += " st: " + std::to_string(isStill()) + "\n";
    return message;
}
