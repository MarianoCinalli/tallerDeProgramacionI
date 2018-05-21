#include "controller/actions/Action.h"

Action::Action() {
  // deberia haber un constructor con el numero de jugador?
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
