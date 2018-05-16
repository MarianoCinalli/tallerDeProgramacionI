#include <SDL2/SDL_ttf.h>
#include "view/Texture.h"

Texture::Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt) {
    this->sdlTexture = this->loadSheet(sheetPath, renderer, transparency, shirt);
}

Texture::Texture(std::string sheetPath, std::string shirtPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt) {
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
    SDL_Surface* shirtSurface = IMG_Load(shirtPath.c_str());

    if (loadedSurface == NULL) {
        std::string error = "Texture: No se pudo cargar la imagen '"
                            + path + "'! SDL Error: "  + IMG_GetError();
        log(error, LOG_ERROR);
    } else if (shirtSurface == NULL) {
        std::string error = "Texture: No se pudo cargar la imagen '"
                            + shirtPath + "'! SDL Error: "  + IMG_GetError();
        log(error, LOG_ERROR);
    } else {
        if (transparency != NULL) {
            //Color key image (Le saca el fondo verde 00a000 en exa)
            log("Texture: Aplicando transparencia a los sprites de los colores: ", transparency, LOG_INFO);
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
            log("Texture: Aplicando transparencia a superficie de las remeras de los colores: ", transparency, LOG_INFO);
            SDL_SetColorKey(
                shirtSurface,
                SDL_TRUE,
                SDL_MapRGB(
                    shirtSurface->format,
                    transparency->getRed(),
                    transparency->getGreen(),
                    transparency->getBlue()
                )
            );
        }
        log("Texture: Pintando remeras de color: ", shirt, LOG_INFO);
        SDL_SetSurfaceColorMod(
            shirtSurface,
            shirt->getRed(),
            shirt->getGreen(),
            shirt->getBlue()
        );

        SDL_BlitSurface(shirtSurface, NULL, loadedSurface, NULL);
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
        SDL_FreeSurface(shirtSurface);
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

Texture::Texture(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont) {
    this->sdlTexture = this->loadFromRenderedText(textureText, renderer, textColor, gFont);
}

Texture::Texture() {
    
}

SDL_Texture* Texture::loadFromRenderedText( std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont )
{
	//Get rid of preexisting texture
	//free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
    this->sdlTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( this->sdlTexture == NULL )
		{
      log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError(), LOG_INFO);
		}
		else
		{
			//Get image dimensions
			this->width = textSurface->w;
			this->height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
    log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
	}

	//Return success
	return this->sdlTexture;
}

// void Texture::render( int x, int y )
// {
// 	//Set rendering space and render to screen
// 	SDL_Rect renderQuad = { x, y, this->width, this->height };
//
// 	//Render to screen
// 	SDL_RenderCopyEx( this->renderer, this->sdlTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE );
// }

int Texture::getWidth() {
	return this->width;
}

int Texture::getHeight() {
	return this->height;
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
