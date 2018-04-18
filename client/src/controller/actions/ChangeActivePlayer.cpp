#include "controller/actions/ChangeActivePlayer.h"

ChangeActivePlayer::ChangeActivePlayer() {
}

void ChangeActivePlayer::execute(Pitch* pitch) {
    log("ChangeActivePlayer: Cambiando el jugador activo.", LOG_DEBUG);
    Player* lastActivePlayer = pitch->activePlayer;
    pitch->changeActivePlayer();
    if(pitch->activePlayer!=lastActivePlayer){
        // El lastActivePlayer debe regresar a base position
        lastActivePlayer->returnToBasePosition();
        lastActivePlayer->stop(pitch->activePlayer->getOrientation());
        // lastActivePlayer->toggleIsSelected();
        // pitch->activePlayer->toggleIsSelected();
    }
}

bool ChangeActivePlayer::valid(Pitch* pitch){
  return true;
}

ChangeActivePlayer::~ChangeActivePlayer() {
}
