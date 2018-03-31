#include "view/SpriteManager.h"

SpriteManager::SpriteManager(SpriteSheet* spriteSheet) {
    this->spriteSheet = spriteSheet;
}

void SpriteManager::render(SDL_Renderer* screen, Coordinates* position) {
    // Todo.
}

SpriteManager::~SpriteManager() {
    // No se borra el sprite sheet aca porque esta compartido entre todos los jugadores.
    // Se borra en el main.
}


