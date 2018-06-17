#include "controller/actions/HighKickingAction.h"

HighKickingAction::HighKickingAction(int power) {
    this->power = power;
}

void HighKickingAction::execute(Player* player) {
    log("HighKickingAction: Tecla patear detectada.", LOG_DEBUG);
    bool highPass = true;
    player->startsKicking(this->power, highPass);
}

bool HighKickingAction::valid(Player* player) {
    return true;
}

HighKickingAction::~HighKickingAction() {
}
