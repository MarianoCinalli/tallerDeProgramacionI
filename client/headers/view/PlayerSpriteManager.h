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
        SDL_Rect sprite;
    public:
        PlayerSpriteManager(Texture* spriteSheet, Player* player);
        // Dibuja un jugador, en las coordenadas del render que se le pase.
        void render(SDL_Renderer* screen, Coordinates* coordinates);
        Coordinates* getPlayerCoordinates();
        ~PlayerSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);
        //para sprite de jugador parado.
        void setStandingSprite(int orientation);
        // Para los sprites del jugador corriendo.
        void setSprite(Velocity* velocity, bool sliding, bool kicking);
        void setRunningSprite(Velocity* velocity);
        bool isRunningUp(int xComponent, int yComponent);
        void setRunningUpSprite();
        bool isRunningDown(int xComponent, int yComponent);
        void setRunningDownSprite();
        bool isRunningLeft(int xComponent, int yComponent);
        void setRunningLeftSprite();
        bool isRunningRight(int xComponent, int yComponent);
        void setRunningRightSprite();

        void setSlidingSprite();
        void setKickingSprite();

        // Para el marcador de jugador activo.
        SDL_Rect getActivePlayerMarker();
        SDL_Rect getActivePlayerMarkerPosition(Coordinates* coordinates);
};
#endif // PLAYERSPRITEMANAGER_H
