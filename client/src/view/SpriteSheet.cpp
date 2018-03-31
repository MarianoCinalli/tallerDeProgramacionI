#include "view/SpriteSheet.h"

SpriteSheet::SpriteSheet(std::string sheetPath, SDL_Renderer* renderer) {
    this->spriteSheetTexture = this->loadSheet(sheetPath, renderer);
}

SDL_Texture* SpriteSheet::getSpriteSheetTexture() {
    return this->spriteSheetTexture;
}

SpriteSheet::~SpriteSheet() {
    this->free();
}

SDL_Texture* SpriteSheet::loadSheet(std::string path, SDL_Renderer* renderer) {
    //Get rid of preexisting texture
    this->free();
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Color key image (Le saca el fondo verde 00a000 en exa)
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xa0, 0));
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            this->width = loadedSurface->w;
            this->height = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void SpriteSheet::free() {
    //Free texture if it exists
    if (this->spriteSheetTexture != NULL) {
        SDL_DestroyTexture(this->spriteSheetTexture);
        this->spriteSheetTexture = NULL;
        this->width = 0;
        this->height = 0;
    }
}
