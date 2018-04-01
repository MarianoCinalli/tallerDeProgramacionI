#include "controller/ActionsManager.h"

ActionsManager::ActionsManager() {
}

ActionsManager::~ActionsManager() {
}

// Devuelve la accion correspondiente a un evento.
Action* ActionsManager::getAction(SDL_Event event) {
    Action* action = NULL;
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        // Actions for pressed keys.
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
        }
    } else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        // Actions for released keys.
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
