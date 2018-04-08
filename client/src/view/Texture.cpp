#include "view/Texture.h"

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, transparency);
}

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, NULL);
}

SDL_Texture* Texture::getSpriteSheetTexture() {
    return this->sdlTexture;
}

Texture::~Texture() {
    this->free();
}

SDL_Texture* Texture::loadSheet(std::string path, SDL_Renderer* renderer, Colour* transparency) {
    //Get rid of preexisting texture
    this->free();
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        if (transparency != NULL) {
            //Color key image (Le saca el fondo verde 00a000 en exa)
            SDL_SetColorKey(
                loadedSurface,
                SDL_TRUE,
                SDL_MapRGB(
                    loadedSurface->format,
                    transparency->getRed(),
                    transparency->getGreen(),
                    transparency->getBlue()
                )
            );
        }
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

void Texture::free() {
    //Free texture if it exists
    if (this->sdlTexture != NULL) {
        SDL_DestroyTexture(this->sdlTexture);
        this->sdlTexture = NULL;
        this->width = 0;
        this->height = 0;
    }
}
