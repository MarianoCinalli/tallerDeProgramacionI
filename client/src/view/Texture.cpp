#include "view/Texture.h"

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, transparency, shirt);
}

Texture::Texture(std::string sheetPath, std::string shirtPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt){
  this->sdlTexture = this->loadSheet(sheetPath, shirtPath, renderer, transparency, shirt);
}

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, transparency, NULL);
}

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, NULL, NULL);
}



SDL_Texture* Texture::getSpriteSheetTexture() {
    return this->sdlTexture;
}

Texture::~Texture() {
    this->free();
}


SDL_Texture* Texture::loadSheet(std::string path, std::string shirtPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt) {
    log("Texture: Cargando textura...", LOG_INFO);
    //Get rid of preexisting texture
    this->free();
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    log("Texture: Cargando superficie...", LOG_INFO);
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Surface* loadedSurface2 = IMG_Load(shirtPath.c_str());

    if (loadedSurface == NULL) {
        std::string error = "Texture: No se pudo cargar la imagen '"
            + path + "'! SDL Error: "  + IMG_GetError();
        log(error, LOG_ERROR);
    } else if (loadedSurface2 == NULL) {
        std::string error = "Texture: No se pudo cargar la imagen '"
            + shirtPath + "'! SDL Error: "  + IMG_GetError();
        log(error, LOG_ERROR);
    } else {
        if (transparency != NULL) {
            //Color key image (Le saca el fondo verde 00a000 en exa)
            log("Texture: Aplicando transparencia...", LOG_INFO);
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
            log("Texture: Aplicando transparencia 2...", LOG_INFO);
            SDL_SetColorKey(
                loadedSurface2,
                SDL_TRUE,
                SDL_MapRGB(
                    loadedSurface->format,
                    transparency->getRed(),
                    transparency->getGreen(),
                    transparency->getBlue()
                )
            );
        }
        SDL_SetSurfaceColorMod(loadedSurface2,
          shirt->getRed(),
          shirt->getGreen(),
          shirt->getBlue()
        );

        SDL_BlitSurface(loadedSurface2, NULL, loadedSurface, NULL);
        //Create texture from surface pixels
        log("Texture: Creando textura desde superficie...", LOG_INFO);
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            std::string error = "Texture: No se pudo cargar la textura '"
                + path + "'! SDL Error: "  + SDL_GetError();
            log(error, LOG_ERROR);
        } else {
            //Get image dimensions
            this->width = loadedSurface->w;
            this->height = loadedSurface->h;
            // if (shirt != NULL) {
            //     SDL_SetTextureColorMod(
            //         newTexture,
            //         shirt->getRed(),
            //         shirt->getGreen(),
            //         shirt->getBlue()
            //     );

        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
        SDL_FreeSurface(loadedSurface2);
    }
    log("Texture: Textura cargada.", LOG_INFO);
    return newTexture;
}


SDL_Texture* Texture::loadSheet(std::string path, SDL_Renderer* renderer, Colour* transparency, Colour* shirt) {
    log("Texture: Cargando textura...", LOG_INFO);
    //Get rid of preexisting texture
    this->free();
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    log("Texture: Cargando superficie...", LOG_INFO);
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::string error = "Texture: No se pudo cargar la imagen '"
            + path + "'! SDL Error: "  + IMG_GetError();
        log(error, LOG_ERROR);
    } else {
        if (transparency != NULL) {
            //Color key image (Le saca el fondo verde 00a000 en exa)
            log("Texture: Aplicando transparencia...", LOG_INFO);
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
        log("Texture: Creando textura desde superficie...", LOG_INFO);
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            std::string error = "Texture: No se pudo cargar la textura '"
                + path + "'! SDL Error: "  + SDL_GetError();
            log(error, LOG_ERROR);
        } else {
            //Get image dimensions
            this->width = loadedSurface->w;
            this->height = loadedSurface->h;
            if (shirt != NULL) {
                SDL_SetTextureColorMod(
                    newTexture,
                    shirt->getRed(),
                    shirt->getGreen(),
                    shirt->getBlue()
                );
            }
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    log("Texture: Textura cargada.", LOG_INFO);
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
