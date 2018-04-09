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
            case SDLK_SLIDE:
                action = new StandedAction(SLIDE_ACTION);
                break;
            case SDLK_KICK:
                action = new StandedAction(KICK_ACTION);
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
        /*    case SDLK_SLIDE:
                break;
            case SDLK_KICK:
                break;  */
        }
    }
    return action;
}
