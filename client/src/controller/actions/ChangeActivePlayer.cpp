#include "controller/actions/ChangeActivePlayer.h"

ChangeActivePlayer::ChangeActivePlayer() {
}

void ChangeActivePlayer::execute(Pitch* pitch) {
    log("ChangeActivePlayer: Cambiando el jugador activo.", LOG_DEBUG);
    pitch->changeActivePlayer();
}

bool ChangeActivePlayer::valid(Pitch* pitch){
  return true;
}

std::string ChangeActivePlayer::getCommand() {
    return "changeActivePlayer";
}

std::vector<std::string> ChangeActivePlayer::getParameters() {
    std::vector<std::string> params;
    return params;
}

ChangeActivePlayer::~ChangeActivePlayer() {
}
