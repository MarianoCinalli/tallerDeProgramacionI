#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class SpriteManager {
    private:
    public:
        SpriteManager();
        render(SDL_Renderer* screen, Coordinates* position);
        ~SpriteManager();
};
#endif // SPRITEMANAGER_H


