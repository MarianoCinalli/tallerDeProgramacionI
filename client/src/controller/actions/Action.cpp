#include "controller/actions/Action.h"

Action::Action() {
}

void Action::execute(Player* player) {
    log("Llamando al execute de Action!", LOG_ERROR);
}

void Action::execute(Pitch* pitch){
  log("Llamando al execute de Action!", LOG_ERROR);
}

bool Action::valid(Player* player) {
  return false;
}

bool Action::valid(Pitch* pitch) {
  return false;
}

Action::~Action() {
}
