#include "controller/actions/ChangeActivePlayer.h"

ChangeActivePlayer::ChangeActivePlayer() {
}

std::string ChangeActivePlayer::getCommand() {
    return "ChangeActivePlayer";
}

std::string ChangeActivePlayer::getParameters() {
    return "";
}

ChangeActivePlayer::~ChangeActivePlayer() {
}
