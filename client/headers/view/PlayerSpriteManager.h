#include "util/Coordinates.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "view/Texture.h"
#include "model/Player.h"
#include <SDL2/SDL.h>
#include <functional>

#ifndef PLAYERSPRITEMANAGER_H
#define PLAYERSPRITEMANAGER_H

// Contiene el sprite sheet.
// Sabe dibujar el sprite actual.
class PlayerSpriteManager {
    private:
        Player* player;
        Texture* spriteSheet;
        SDL_Rect sprite;
        SDL_Rect activePlayerMarker;
    public:
        PlayerSpriteManager(Texture* spriteSheet, Player* player);
        // Dibuja un jugador, en las coordenadas del render que se le pase.
        void render(SDL_Renderer* screen, Coordinates* coordinates);
        Coordinates* getPlayerCoordinates();
        int getPlayerTeam();
        ~PlayerSpriteManager();
    private:
        SDL_Rect getPositionOnScreen(SDL_Rect sprite, Coordinates* coordinates);

        //contadores
        int standingCount;
        int kickingCount;
        int slidingCount;
        int runningCount;
        int activePlayerCount;

        //estado
        bool sliding;
        bool wasSliding;
        bool kicking;
        bool wasKicking;

        // Para los sprites de jugador parado.
        void setStandingSprite(int orientation);
        void setStandingSpriteViewUp();
        void setStandingSpriteViewRight();
        void setStandingSpriteViewDown();
        void setStandingSpriteViewLeft();

        // Para los sprites del jugador corriendo.
        void setSprite(bool sliding, bool kicking, bool runningFast);
        void setRunningSprite(bool runningFast);
        bool isRunningUp(int xComponent, int yComponent);
        void setRunningUpSprite(int cteVelocidad);
        bool isRunningDown(int xComponent, int yComponent);
        void setRunningDownSprite(int cteVelocidad);
        bool isRunningLeft(int xComponent, int yComponent);
        void setRunningLeftSprite(int cteVelocidad);
        bool isRunningRight(int xComponent, int yComponent);
        void setRunningRightSprite(int cteVelocidad);

        void setSlidingSprite();
        void setKickingSprite();

        void startsKicking();
        void stopKicking();
        bool isKicking();
        bool wasKickingYet();
        void isAlreadyKicking();
        bool isSliding();
        bool wasSlidingYet();
        void startsSliding();
        void isAlreadySliding();
        void stopSliding();

        // Para el marcador de jugador activo.
        SDL_Rect getActivePlayerMarker();
        SDL_Rect getActivePlayerMarkerPosition(Coordinates* coordinates);
};
#endif // PLAYERSPRITEMANAGER_H
