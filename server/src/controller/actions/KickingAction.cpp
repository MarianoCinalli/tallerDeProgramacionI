#include "controller/actions/KickingAction.h"

KickingAction::KickingAction(int power, bool highPass) {
    this->power = power;
    this->highPass = highPass;
}

void KickingAction::execute(Player* player) {
    log("KickingAction: Tecla patear detectada.", LOG_DEBUG);
    player->startsKicking(this->power, this->highPass);
}

bool KickingAction::valid(Player* player) {
    return true;
}

KickingAction::~KickingAction() {
	//preguntar por esto!
}
