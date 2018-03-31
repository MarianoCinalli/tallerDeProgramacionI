#include "util/Coordinates.h"
#include "view/SpriteSheet.h"
#include "view/Player.h"
#include <SDL2/SDL.h>

#ifndef PLAYERSPRITEMANAGER_H
#define PLAYERSPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class PlayerSpriteManager {
    private:
        SpriteSheet* spriteSheet;
    public:
        PlayerSpriteManager(SpriteSheet* spriteSheet);
        void render(Player* player, SDL_Renderer* screen, Coordinates* position);
        ~PlayerSpriteManager();
};
#endif // PLAYERSPRITEMANAGER_H
