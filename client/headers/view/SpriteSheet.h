#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#ifndef SPRITESHEET_H
#define SPRITESHEET_H

class SpriteSheet {
    private:
        SDL_Texture* spriteSheetTexture;
        int height;
        int width;
    public:
        SpriteSheet(std::string sheetPath, SDL_Renderer* renderer);
        SDL_Texture* getSpriteSheetTexture();
        ~SpriteSheet();
        SDL_Texture* loadSheet(std::string path, SDL_Renderer* renderer);
        void free();
};
#endif // SPRITESHEET_H
