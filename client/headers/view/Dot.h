#ifndef DOT_H
#define DOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "view/LTexture.h"

class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot(void);

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move(void);

		//Shows the dot on the screen relative to the camera
		void render(SDL_Renderer* gRenderer, LTexture* gDotTexture, int camX, int camY );

		//Position accessors
		int getPosX(void);
		int getPosY(void);

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
};

#endif // DOT_H
