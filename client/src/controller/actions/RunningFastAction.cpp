#include "controller/actions/RunningFastAction.h"

RunningFastAction::RunningFastAction() {
    //TODO ver clase
}

void RunningFastAction::execute(Player* player) {
    log("RunningFastAction: Tecla de correr rapido detectada.", LOG_DEBUG);
    player->startsRunningFast();
}

bool RunningFastAction::valid(Player* player) {
    return true;
}

std::string RunningFastAction::getCommand() {
    return "RunningFast";
}

std::string RunningFastAction::getParameters() {
    return "";
}

RunningFastAction::~RunningFastAction() {
	//preguntar por esto!
}
