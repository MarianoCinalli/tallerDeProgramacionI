#include "controller/actions/Stop.h"

Stop::Stop() {
}

void Stop::execute(Player* player) {
    log("Stop: frenando al jugador.", LOG_DEBUG);
    player->stop();
}

bool Stop::valid(Player* player){
  return true;
}

std::string Stop::getCommand() {
    return "stop";
}

std::vector<std::string> Stop::getParameters() {
    std::vector<std::string> params;
    return params;
}

Stop::~Stop() {
}
