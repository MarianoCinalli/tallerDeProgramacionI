#include "controller/actions/KickingAction.h"

KickingAction::KickingAction(int power) {
    this->power = power;
}

void KickingAction::execute(Player* player) {
    log("KickingAction: Tecla patear detectada.", LOG_DEBUG);
    bool highPass = false;
    player->startsKicking(this->power, highPass);
}

bool KickingAction::valid(Player* player) {
    return true;
}

KickingAction::~KickingAction() {
	//preguntar por esto!
}
