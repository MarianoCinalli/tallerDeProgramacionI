#include "controller/actions/KickingAction.h"

KickingAction::KickingAction() {
    //TODO ver clase
}

void KickingAction::execute(Player* player) {
    log("StandedAction: Tecla deslizar detectad.", LOG_DEBUG);
    player->startsKicking();
}

KickingAction::~KickingAction() {
	//preguntar por esto!
}
