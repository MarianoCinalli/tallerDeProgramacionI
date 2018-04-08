#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/Texture.h"
#include "model/Player.h"
#include <SDL2/SDL.h>

#ifndef PLAYERSPRITEMANAGER_H
#define PLAYERSPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class PlayerSpriteManager {
    private:
        Player* player;
        Texture* spriteSheet;
        int spriteWidth;
        int spriteHeight;
        SDL_Rect sprite;
    public:
        PlayerSpriteManager(Texture* spriteSheet, Player* player);
        // Dibuja un jugador, en las coordenadas del render que se le pase.
        void render(SDL_Renderer* screen, Coordinates* coordinates);
        Coordinates* getPlayerCoordinates();
        ~PlayerSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);
        void setStandingSprite(int orientation);

        // Para los sprites del jugador corriendo.
        void setRunningSprite(Velocity* velocity);
        bool isRunningUp(int xComponent, int yComponent);
        void setRunningUpSprite();
        bool isRunningDown(int xComponent, int yComponent);
        void setRunningDownSprite();
        bool isRunningLeft(int xComponent, int yComponent);
        void setRunningLeftSprite();
        bool isRunningRight(int xComponent, int yComponent);
        void setRunningRightSprite();
};
#endif // PLAYERSPRITEMANAGER_H
