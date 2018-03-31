#include "view/PlayerSpriteManager.h"

PlayerSpriteManager::PlayerSpriteManager(SpriteSheet* spriteSheet) {
    this->spriteSheet = spriteSheet;
}

void PlayerSpriteManager::render(Player* player, SDL_Renderer* screen, Coordinates* position) {
    // Todo.
}

PlayerSpriteManager::~PlayerSpriteManager() {
    delete(this->spriteSheet);
}


