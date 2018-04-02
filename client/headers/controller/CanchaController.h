#ifndef CANCHA_CONTROLLER_H
#define CANCHA_CONTROLLER_H
#include "view/LTexture.h"
#include "view/Dot.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>



class CanchaController
{
public:
	CanchaController(void);
	~CanchaController(void);
	void startView(void);
	bool init(void);
	void close(void);
	bool loadMedia(void);

	//The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    //Scene textures
    LTexture gDotTexture;
    LTexture gBGTexture;

};

#endif // CANCHA_CONTROLLER_H
