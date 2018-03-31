#include "util/Coordinates.h"
#include "view/SpriteSheet.h"
#include <SDL2/SDL.h>

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class SpriteManager {
    private:
        SpriteSheet* spriteSheet;
    public:
        SpriteManager(SpriteSheet* spriteSheet);
        void render(SDL_Renderer* screen, Coordinates* position);
        ~SpriteManager();
};
#endif // SPRITEMANAGER_H


