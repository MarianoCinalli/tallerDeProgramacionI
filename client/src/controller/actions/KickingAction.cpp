#include "controller/actions/KickingAction.h"

KickingAction::KickingAction() {
    //TODO ver clase
}

void KickingAction::execute(Player* player) {
    log("KickingAction: Tecla patear detectada.", LOG_DEBUG);
    player->startsKicking();
}

KickingAction::~KickingAction() {
	//preguntar por esto!
}
