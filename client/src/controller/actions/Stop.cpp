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

Stop::~Stop() {
}
