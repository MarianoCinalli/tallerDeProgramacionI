#include "controller/actions/SlidingAction.h"

SlidingAction::SlidingAction() {
    //TODO ver clase
}

void SlidingAction::execute(Player* player) {
    log("SlidingAction: Tecla deslizar detectada.", LOG_DEBUG);
    player->startsSliding();
}

bool SlidingAction::valid(Player* player) {
    return true;
}

SlidingAction::~SlidingAction() {
	//preguntar por esto!
}
