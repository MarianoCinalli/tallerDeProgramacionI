#include "model/Ball.h"

Ball::Ball(Coordinates* position, float maxSpeed, float decelerate) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->passPower = 0;
    this->initialPassPower = 0;
    this->height = 0;
    this->heightLevel = 0;
    this->orientation = 0;
    this->dominated = true;
    this->velocity = new Velocity(0, 0);
    this->maxSpeed = maxSpeed;
    this->decelerate = decelerate;
    this->isInAPass = false;
    this->isInAHighPass = false;
    this->passDirection = 0;
    this->decelerateLevel = 0;
    this->decelerateDistance = 0;
    this->timeIntercept = 0; // corrector para frames TODO
    this->startingPassPosition = new Coordinates(800, 500); // Esto esta perdiendo memoria?
    this->interceptable = true;
    // this->player = player;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}

void Ball::setPosition(Coordinates* pos){
  this->position->set(*pos);
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

  if ((position == CENTER_LEFT_START) ||  (position == CENTER_LEFT_START)){
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
    this->setPosition(player->getPosition());
    this->player->setWithBall(this->dominated);
    this->velocity = this->player->getVelocity();
}

bool Ball::isDominated() {
    return this->dominated;
}

bool Ball::isInterceptable() {
    return this->interceptable;
}

void Ball::isIntercepted(Player* player) {
    this->timeIntercept = 0;
    this->isInAPass = false;
    this->isInAHighPass = false;
    this->passPower = 0;
    this->height = 0;
    this->heightLevel = 0;
    this->interceptable = false;
    this->setPlayer(player);
    this->orientation = player->getOrientation();
    log("Ball: Pelota interceptada por jugador.", LOG_DEBUG);
}

void Ball::isPassed(Velocity* velocity, float passPower, bool highPass) {
    if (this->isDominated()) {
        this->timeIntercept = 0;
        this->interceptable = false;
        Velocity* passDirection = new Velocity(0,0);
        passDirection->set(velocity);
        this->velocity = passDirection;
        log("BALL: velocidad ", this->velocity, LOG_DEBUG);
        log(this->velocity->toString(),LOG_DEBUG);
        if (passDirection->isZero() || (highPass)) {
            log("Ball: el pase es sin ayuda", this->orientation, LOG_DEBUG);
            this->velocity->set(this->player->getVelocity());
            if (this->velocity->isZero()) {
                log("Ball: jugador tiene velocidad cero", LOG_DEBUG);
                int passOrientation = this->player->getOrientation();
                this->velocity->accelerate(passOrientation);
            }
        }
        this->dominated = false; //HACK? para que sirve?
        this->player->setWithBall(false);
        this->isInAPass = true;
        if(highPass) {
            this->isInAHighPass = true;
            this->passPower = passPower*this->maxSpeed*1.2; //para que sea mas sensible el pase elevado
            this->initialPassPower = this->passPower;
        }
        else{
          this->passPower = passPower*this->maxSpeed;
          this->initialPassPower = this->passPower;
        }
        this->startingPassPosition = this->position;
    }
}


const int TIME_BALL_NO_INTERCEPT = 9;
const int BALL_DECELERATE_TIME = 3; //a numeros mas grandes, tarda mas


const float BALL_HEIGHT_CONST = 1.3;

float Ball::calculatePassPower(float passPower, int timePassing)
{
    float decel = 0.27;
    float finalPassPower;
    float deltaTimePassing = (this->initialPassPower - 3)/decel;
    if (passPower > 4)
    {
        finalPassPower = this->initialPassPower - decel*timePassing;
    }
    else
    {
        finalPassPower = 3 - decel/3 * (timePassing-deltaTimePassing);
    }
    return finalPassPower;
}

float calculateHeight2(float maxPassPower, float passPower)
{
    float X1;
    float X2;
    float roof = 0.2;
    float maxHeight = maxPassPower/2;
    if (maxPassPower < 2)
    {
        X1 = 0;
        X2 = maxPassPower / 2;
    }
    else
    {
        X1 = (maxPassPower * 1 / 4);
        X2 = maxPassPower - X1;
    }
    if (passPower > X2+1)
    {
        return (maxHeight * (maxPassPower - passPower) / (maxPassPower - X2));
    }
    else if((passPower < X2 + roof) && (passPower > X2 -roof)){
        return maxHeight;
    }
    else if (passPower > X1-1)
    {
        return maxHeight * (passPower - X1) / (X2 - X1);
    }
    else
        return 0;
}

void Ball::updatePosition() {
    if (this->isDominated()) {
        this->orientation = player->getOrientation();
        this->calculateDominatedPosition();
    }
    if (this->isInAPass) {
        ++this->timeIntercept;
        if (!this->interceptable && (timeIntercept > TIME_BALL_NO_INTERCEPT)) { //TODO numero harcodeado tiempo de pase
            this->interceptable = true;
        }
        if(this->isInAHighPass && !this->velocity->isZero()) {
            // if(this->height > BALL_HEIGHT_CONST){    ///ya se verifica esto en las funciones
            //     this->interceptable = false;
            // }
           
        }
        log("Ball: paso este tiempo:", this->timeIntercept, LOG_SPAM);
        if(this->position->addX(this->velocity->getComponentX()*this->passPower)<0){
          this->velocity->scaleX(-1);
        }
        if(this->position->addY(this->velocity->getComponentY()*this->passPower)<0){
          this->velocity->scaleY(-1);
        }
        if (!this->velocity->isZero()) {
            if (this->isInAHighPass)
            {
                // this->calculateHeight();
                this->height = calculateHeight2(this->initialPassPower, this->passPower);
                log("Ball: altura del balon: ", this->height, LOG_SPAM);
                log("Ball: poder de pase: ", this->passPower, LOG_SPAM);
            }
            // if (this->timeIntercept % BALL_DECELERATE_TIME == 0) {
            this->passPower = calculatePassPower(this->passPower, this->timeIntercept);
            if (this->passPower< 0){
                this->timeIntercept = 0;
                this->stopRolling();
            }
            // }
        }
    }
    else {
        if(!this->interceptable){
            ++this->timeIntercept;
            if (timeIntercept > TIME_BALL_NO_INTERCEPT*8)   //para que no me la saque al toque que se la saque
            { //TODO numero harcodeado tiempo de pase
                this->interceptable = true;
            }
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
        if (this->height > BALL_HEIGHT_CONST) {
            if ((initialPassPower * 0.6) < passPower && decelerateLevel == 0) {
                this->height = this->height - BALL_HEIGHT_CONST;
                this->decelerateLevel = 1;
            }
            else {
                if((initialPassPower * 0.45) < passPower && decelerateLevel == 1) {
                this->height = this->height - BALL_HEIGHT_CONST;
                this->decelerateLevel = 2;
                }
                else {
                    if((initialPassPower * 0.3) < passPower && decelerateLevel == 2) {
                    this->height = this->height - BALL_HEIGHT_CONST;
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


void Ball::calculateDominatedPosition() {
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
        case PLAYER_ORIENTATION_UP_R:
            x = (x + CUERPO_JUGADOR/2);
            y = (y - CUERPO_JUGADOR/2);
            break;
        case PLAYER_ORIENTATION_DOWN_R:
            x = (x + CUERPO_JUGADOR/2);
            y = (y + CUERPO_JUGADOR/2);
            break;
        case PLAYER_ORIENTATION_DOWN_L:
            x = (x - CUERPO_JUGADOR/2);
            y = (y + CUERPO_JUGADOR/2);
            break;
        case PLAYER_ORIENTATION_UP_L:
            x = (x - CUERPO_JUGADOR/2);
            y = (y - CUERPO_JUGADOR/2);
            break;
    }
    this->position->setX(x);
    this->position->setY(y);
    // Coordinates* newPosition = new Coordinates(x, y);
    // return newPosition;
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
    message += " cx: " + std::to_string((int)this->position->getX()) + "\n";
    message += " cy: " + std::to_string((int)this->position->getY()) + "\n";
    message += " st: " + std::to_string(this->velocity->isZero()) + "\n";
    message += " hg: " + std::to_string(this->height) + "\n";
    message += " or: " + std::to_string(this->orientation) + "\n";
    return message;
}

// Devuelve true si el equipo del jugador tiene la pelota.
bool Ball::doesHisTeamHaveTheBall(Player* somePlayer) {
    return (this->player->getTeam() == somePlayer->getTeam());
}

Ball::~Ball() {
    delete(this->position);
    // delete(this->player);
    delete(this->velocity);
}
