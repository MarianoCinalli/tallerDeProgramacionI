#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "util/Colour.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    private:
        SDL_Texture* sdlTexture;
        int height;
        int width;
    public:
        Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt);
        Texture(std::string sheetPath, std::string shirtPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt);
        Texture(std::string sheetPath, SDL_Renderer* renderer, Colour* transparency);
        Texture(std::string sheetPath, SDL_Renderer* renderer);
        SDL_Texture* getSpriteSheetTexture();
        ~Texture();
        SDL_Texture* loadSheet(std::string path, SDL_Renderer* renderer, Colour* transparency, Colour* shirt);
        SDL_Texture* loadSheet(std::string path, std::string shirtPath, SDL_Renderer* renderer, Colour* transparency, Colour* shirt);

        //Creates image from font string
        Texture(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont);
        SDL_Texture* loadFromRenderedText( std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont );
        //void render( int x, int y );

    		//Gets image dimensions
    		int getWidth();
    		int getHeight();

        void free();
};
#endif // TEXTURE_H
