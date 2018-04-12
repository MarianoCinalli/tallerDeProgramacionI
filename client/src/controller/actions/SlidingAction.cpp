#include "controller/actions/SlidingAction.h"

SlidingAction::SlidingAction() {
    //TODO ver clase
}

void SlidingAction::execute(Player* player) {
    log("StandedAction: Tecla deslizar detectad.", LOG_DEBUG);
    player->startsSliding();
}

SlidingAction::~SlidingAction() {
	//preguntar por esto!
}
