#include "controller/CanchaController.h"
#include "util/Constants.h"
#include "view/LTexture.h"
#include "view/Dot.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "util/logger.h"


CanchaController::CanchaController(void){
}


CanchaController::~CanchaController(void){
}



void CanchaController::startView(void){
     //Start up SDL and create window
	if( !init() )
	{
		log( "Failed to initialize!\n", logLevels::LOG_ERROR );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			log( "Failed to load media!\n", logLevels::LOG_ERROR );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//The margin area
			SDL_Rect margin = { camera.x + 20, camera.y + 20, camera.w - 40, camera.h - 40 };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot
				dot.move();

				//Center the camera over the dot
				camera.x = ( dot.getPosX() + Dot::DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( dot.getPosY() + Dot::DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

				// Se pasa del margen derecho
				// if( ( dot.getPosX() + Dot::DOT_WIDTH ) > ( margin.w + 20 ) )
				// {
				//     printf("1 getPosX %d\n", dot.getPosX());
				//  	camera.x = ( dot.getPosX() + Dot::DOT_WIDTH ) - ( margin.w + 20 );
				//  	margin.x = camera.x + 20;
				// }

				// if( dot.getPosX() < margin.x )
				// {
				//     printf("2 getPosX %d\n", dot.getPosX());
				//  	camera.x = dot.getPosX() - margin.x;
				//  	margin.x = camera.x + 20;
				// }

				//Keep the camera in bounds
				if( camera.x < 0 )
				{
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( gRenderer, 0, 0, &camera );

				//Render objects
				dot.render(gRenderer, &gDotTexture, camera.x, camera.y );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return;

}

bool CanchaController::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError(), logLevels::LOG_ERROR );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			log( "Warning: Linear texture filtering not enabled!", logLevels::LOG_ERROR );
		}

		//Create window
		gWindow = SDL_CreateWindow( "ZIDANE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			log( "Window could not be created! SDL Error: %s\n", SDL_GetError(), logLevels::LOG_ERROR );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError(), logLevels::LOG_ERROR );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					log( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError(), logLevels::LOG_ERROR );
					success = false;
				}
			}
		}
	}

	return success;
}

void CanchaController::close()
{
	//Free loaded images
	gDotTexture.free();
	gBGTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool CanchaController::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile(gRenderer, "images/dot.bmp" ) )
	{
		log( "Failed to load dot texture!\n", logLevels::LOG_ERROR );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile(gRenderer, "images/bg.png" ) )
	{
		log( "Failed to load background texture!\n", logLevels::LOG_ERROR );
		success = false;
	}

	return success;
}
