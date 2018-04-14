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
            case SDLK_UP:
                action = new Decelerate(PLAYER_ORIENTATION_UP);
                break;
            case SDLK_DOWN:
                action = new Decelerate(PLAYER_ORIENTATION_DOWN);
                break;
            case SDLK_LEFT:
                action = new Decelerate(PLAYER_ORIENTATION_LEFT);
                break;
            case SDLK_RIGHT:
                action = new Decelerate(PLAYER_ORIENTATION_RIGHT);
                break;
        }
    }
    return action;
}
