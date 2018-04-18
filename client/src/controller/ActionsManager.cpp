#include "controller/ActionsManager.h"

ActionsManager::ActionsManager() {
    log("ActionsManager: Creando actionsManager.", LOG_INFO);
}

ActionsManager::~ActionsManager() {
    log("ActionsManager: Eliminando actionsManager.", LOG_INFO);
}

// Devuelve la accion correspondiente a un evento.
Action* ActionsManager::getAction(SDL_Event event) {
    Action* action = NULL;
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
                break;
            case SDLK_DOWN:
                action = new Accelerate(PLAYER_ORIENTATION_DOWN);
                break;
            case SDLK_LEFT:
                action = new Accelerate(PLAYER_ORIENTATION_LEFT);
                break;
            case SDLK_RIGHT:
                action = new Accelerate(PLAYER_ORIENTATION_RIGHT);
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
        switch (event.key.keysym.sym) {
            case SDLK_w:
                action = new StopRunningFastAction();
                break;
            case SDLK_UP:
                action = new StopRunningAction(PLAYER_ORIENTATION_UP);
                break;
            case SDLK_DOWN:
                action = new StopRunningAction(PLAYER_ORIENTATION_DOWN);
                break;
            case SDLK_LEFT:
                action = new StopRunningAction(PLAYER_ORIENTATION_LEFT);
                break;
            case SDLK_RIGHT:
                action = new StopRunningAction(PLAYER_ORIENTATION_RIGHT);
                break;
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
