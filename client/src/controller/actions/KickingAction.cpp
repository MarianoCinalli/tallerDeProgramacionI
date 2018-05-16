#include "controller/actions/KickingAction.h"

KickingAction::KickingAction() {
    //TODO ver clase
}

void KickingAction::execute(Player* player) {
    log("KickingAction: Tecla patear detectada.", LOG_DEBUG);
    player->startsKicking();
}

bool KickingAction::valid(Player* player) {
    return true;
}

std::string KickingAction::getCommand() {
    return "Kicking";
}

std::string KickingAction::getParameters() {
    return "";
}

KickingAction::~KickingAction() {
	//preguntar por esto!
}
