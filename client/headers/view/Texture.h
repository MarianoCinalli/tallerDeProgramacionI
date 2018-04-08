#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    private:
        SDL_Texture* sdlTexture;
        int height;
        int width;
    public:
        Texture(std::string sheetPath, SDL_Renderer* renderer);
        SDL_Texture* getSpriteSheetTexture();
        ~Texture();
        SDL_Texture* loadSheet(std::string path, SDL_Renderer* renderer);
        void free();
};
#endif // TEXTURE_H
