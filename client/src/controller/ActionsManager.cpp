#include "controller/ActionsManager.h"

ActionsManager::ActionsManager() {
    log("ActionsManager: Creando actionsManager.", LOG_INFO);
}

ActionsManager::~ActionsManager() {
    log("ActionsManager: Eliminando actionsManager.", LOG_INFO);
}

bool anyKeyPressed(bool* keys){
  log("TECLAS", LOG_DEBUG);
  // int i = 0;
  // for (bool b : keys){
  for(int i = 0; i < 4; i++){
    if (keys[i]){
      log("Tecla: ", i,LOG_DEBUG);
      return true;
    }
  }
  return false;
}

// Devuelve la accion correspondiente a un evento.
Action* ActionsManager::getAction(SDL_Event event) {
    Action* action = NULL;
    bool* keys = this->keys;
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        // Actions for pressed keys.
        log("ActionsManager: Se registro una tecla presionada.", LOG_DEBUG);
        switch (event.key.keysym.sym) {
            case SDLK_w:
                action = new RunningFastAction();
                break;
            case SDLK_a:
                action = new SlidingAction();
                break;
            case SDLK_s:
                action = new KickingAction();
                break;
            case SDLK_SPACE:
                action = new ChangeActivePlayer();
                break;
            case SDLK_UP:
                action = new Accelerate(PLAYER_ORIENTATION_UP);
                keys[KUP] = true;
                break;
            case SDLK_DOWN:
                action = new Accelerate(PLAYER_ORIENTATION_DOWN);
                keys[KDOWN] = true;
                break;
            case SDLK_LEFT:
                action = new Accelerate(PLAYER_ORIENTATION_LEFT);
                keys[KLEFT] = true;
                break;
            case SDLK_RIGHT:
                action = new Accelerate(PLAYER_ORIENTATION_RIGHT);
                keys[KRIGHT] = true;
                break;
            default:
                log(
                    "ActionsManager: Tecla erronea presionada: ",
                    SDL_GetKeyName(event.key.keysym.sym),
                    LOG_DEBUG
                );
        }
    } else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        // Actions for released keys.
        log("ActionsManager: Se registro una tecla soltada.", LOG_DEBUG);
        if (event.key.keysym.sym== SDLK_w){
          return new StopRunningFastAction();
        } else {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                keys[KUP] = false;
                action = new Accelerate(PLAYER_ORIENTATION_DOWN);
                break;
            case SDLK_DOWN:
                keys[KDOWN] = false;
                action = new Accelerate(PLAYER_ORIENTATION_UP);
                break;
            case SDLK_LEFT:
                keys[KLEFT] = false;
                action = new Accelerate(PLAYER_ORIENTATION_RIGHT);
                break;
            case SDLK_RIGHT:
                keys[KRIGHT] = false;
                action = new Accelerate(PLAYER_ORIENTATION_LEFT);
                break;
        }
        if (!anyKeyPressed(keys)){
          delete(action);
          action = new Stop();
        }
      }
    }

    return action;
}


bool ActionsManager::shouldQuit(SDL_Event event) {
    bool quit = false;
    bool quitByQ = ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_q));
    bool quitByWindowExit = (event.type == SDL_QUIT);
    if ( quitByWindowExit || quitByQ ) {
        log("ActionsManager: Se registro la salida del programa.", LOG_INFO);
        quit = true;
    }
    return quit;
}
