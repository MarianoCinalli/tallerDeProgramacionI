#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <stdio.h>
#include "util/logger.h"

#include "controller/cancha_controller.h"
#include "model/ltexture.h"
#include "model/dot.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture( gRenderer );
LTexture gBGTexture( gRenderer );

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        log( "SDL could not initialize! SDL Error:", SDL_GetError(), 1 );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            log( "Warning: Linear texture filtering not enabled!", 1 );
        }

        //Create window
        gWindow = SDL_CreateWindow( "ZIDANE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            log( "Window could not be created! SDL Error: ", SDL_GetError(), 1 );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
           = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if == NULL )
            {
                log( "Renderer could not be created! SDL Error: ", SDL_GetError(), 1 );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    log( "SDL_image could not initialize! SDL_image Error: ", IMG_GetError(), 1 );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load dot texture
    if( !gDotTexture.loadFromFile( "images/dot.bmp" ) )
    {
        log( "Failed to load dot texture!", 1 );
        success = false;
    }

    //Load background texture
    if( !gBGTexture.loadFromFile( "images/bg.png" ) )
    {
        log( "Failed to load background texture!", SDL_GetError(), 1 );
        success = false;
    }

    return success;
}

void close() {
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

int startView() {
    //Start up SDL and create window
    if( !init() )
    {
        log( "Failed to initialize!", 1 );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            log( "Failed to load media!", 1 );
        }
        else
        {   
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //The dot that will be moving around on the screen
            Dot dot();

            //The camera area
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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
                gBGTexture.render( 0, 0, &camera );

                //Render objects
                dot.render( camera.x, camera.y );

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    //Free resources and close SDL
    close();
}