#include "controller/ActionsManager.h"
#include <SDL2/SDL_mixer.h>

extern Mix_Music *gMusic;

ActionsManager::ActionsManager() {
    log("ActionsManager: Creando actionsManager.", LOG_INFO);
}

ActionsManager::~ActionsManager() {
    log("ActionsManager: Eliminando actionsManager.", LOG_INFO);
}

bool anyKeyPressed(bool* keys) {
    // int i = 0;
    // for (bool b : keys){
    for (int i = 0; i < 4; i++) {
        if (keys[i]) {
            log("ActionsManager: Tecla presionada: ", i, LOG_SPAM);
            return true;
        }
    }
    return false;
}


int customMap(int val){   //funcion para mapear la potencia del tiro, superhardcode
  if (val<150){
    return 3;
  }
  else if (val <250){
    return 4;   //de 1 a 5
  }
  else if (val <350){
    return 5;   //de 1 a 5
  }
  else{
    return 6;
  }
}

// Devuelve la accion correspondiente a un evento.
Action* ActionsManager::getAction(SDL_Event event) {
    Action* action = NULL;
    bool* keys = this->keys;
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        // Actions for pressed keys.
        log("ActionsManager: Se registro una tecla presionada.", LOG_SPAM);
        switch (event.key.keysym.sym) {
            case SDLK_m:
                //If there is no music playing
                if ( Mix_PlayingMusic() == 0 ) {
                    Mix_PlayMusic( gMusic, -1 ); //Play the music
                } else {
                    //If music is being played
                    if ( Mix_PausedMusic() == 1 ) {
                        Mix_ResumeMusic(); //Resume the music
                    } else {
                        Mix_PauseMusic(); //Pause the music
                    }
                }
                break;
            case SDLK_w:
                action = new RunningFastAction();
                break;
            case SDLK_a:
                action = new SlidingAction();
                break;
            case SDLK_s:
            {
                this->kickTime = SDL_GetTicks();
                break;
              }
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
        log("ActionsManager: Se registro una tecla soltada.", LOG_SPAM);
        if (event.key.keysym.sym == SDLK_w) {
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
                case SDLK_s:
                {
                    int currentTime = SDL_GetTicks();
                    currentTime = currentTime - this->kickTime;
                    int passPower = customMap(currentTime);    //TODO constante a definir, cuanto pass power
                    action = new KickingAction(passPower);
                    log("poder de pase", passPower, LOG_DEBUG);
                    log("tiempo de pase", currentTime, LOG_DEBUG);
                    break;
                  }
            }
            if (!(event.key.keysym.sym == SDLK_s) & !anyKeyPressed(keys)) {
                if (action != NULL) {
                    delete(action);
                }
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
    if (quitByWindowExit || quitByQ) {
        log("ActionsManager: Se registro la salida del programa.", LOG_INFO);
        quit = true;
    }
    return quit;
}
