#include "controller/actions/SlidingAction.h"

SlidingAction::SlidingAction() {
    //TODO ver clase
}

void SlidingAction::execute(Player* player) {
    log("SlidingAction: Tecla deslizar detectada.", LOG_DEBUG);
    player->startsSliding();
}

void SlidingAction::execute(Pitch* pitch) {
    log("SlidingAction: Tecla deslizar detectada.", LOG_DEBUG);
    pitch->checkIntercepts();
}

bool SlidingAction::valid(Player* player) {
    return true;
}

bool SlidingAction::valid(Pitch* pitch) {
    return true;
}

SlidingAction::~SlidingAction() {
	//preguntar por esto!
}
