#include "controller/actions/StopRunningAction.h"

StopRunningAction::StopRunningAction(int direction) {
    this->direction = direction;
}

void StopRunningAction::execute(Player* player) {
    log("StopRunningAction: Frenando al jugador.", LOG_DEBUG);
    player->stopRunningInDirection(this->direction);
}


bool StopRunningAction::valid(Player* player){
  return true;
}

StopRunningAction::~StopRunningAction() {
}
