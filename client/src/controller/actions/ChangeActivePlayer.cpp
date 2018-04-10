#include "controller/actions/ChangeActivePlayer.h"

ChangeActivePlayer::ChangeActivePlayer() {
}

void ChangeActivePlayer::execute(Pitch* pitch) {
    log("ChangeActivePlayer: Cambiando el jugador activo.", LOG_DEBUG);
    pitch->activePlayer->stop(pitch->activePlayer->getOrientation());
    pitch->changeActivePlayer();
}

bool ChangeActivePlayer::valid(Pitch* pitch){
  return true;
}
ChangeActivePlayer::~ChangeActivePlayer() {
}
