#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "view/SpriteSheet.h"
#include "model/Player.h"
#include <SDL2/SDL.h>

#ifndef PLAYERSPRITEMANAGER_H
#define PLAYERSPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class PlayerSpriteManager {
    private:
        SpriteSheet* spriteSheet;
        int spriteWidth;
        int spriteHeight;
        SDL_Rect sprite;
    public:
        PlayerSpriteManager(SpriteSheet* spriteSheet);
        void render(Player* player, SDL_Renderer* screen);
        ~PlayerSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);
        SDL_Rect getStandingSprite(int orientation);
        SDL_Rect getRunningSprite(Velocity* velocity);
};
#endif // PLAYERSPRITEMANAGER_H
