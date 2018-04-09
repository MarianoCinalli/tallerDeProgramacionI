#include "controller/actions/StandedAction.h"

StandedAction::StandedAction(int stdAction) {
    this->action = stdAction;
}

void StandedAction::execute(Player* player) {
	switch (this->action) {
        case SLIDE_ACTION:
            log("StandedAction: jugador se desliza.", LOG_DEBUG);
            player->startSliding();
            break;
        case KICK_ACTION:
            log("StandedAction: Jugador patea.", LOG_DEBUG);
            player->kick();
            break;
	}
}

StandedAction::~StandedAction() {
	//preguntar por esto!
}
