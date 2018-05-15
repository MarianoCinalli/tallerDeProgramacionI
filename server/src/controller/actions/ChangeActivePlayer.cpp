#include "controller/actions/ChangeActivePlayer.h"

ChangeActivePlayer::ChangeActivePlayer() {
}

void ChangeActivePlayer::execute(Pitch* pitch, int user) {
    log("ChangeActivePlayer: Cambiando el jugador activo.", LOG_DEBUG);
    pitch->changeActivePlayer(user);
}

bool ChangeActivePlayer::valid(Pitch* pitch){
  return true;
}

ChangeActivePlayer::~ChangeActivePlayer() {
}
