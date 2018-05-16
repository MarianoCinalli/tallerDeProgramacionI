#include <SDL2/SDL.h> // Esto se deberia borrar.
#include <SDL2/SDL_image.h> // Esto se deberia borrar.
#include <string>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "util/ConnectionManager.h"
#include "util/Coordinates.h"
#include "util/Colour.h"
#include "model/Player.h"
#include "model/Pitch.h"
#include "model/Ball.h"
#include "model/Team.h"
#include "view/PlayerSpriteManager.h" // Esto se deberia borrar.
#include "view/Texture.h" // Esto se deberia borrar.
#include "view/PitchView.h"
#include "view/Camera.h"
#include "controller/GameController.h"
#include "controller/GameControllerProxy.h"
#include "controller/actions/Action.h"

#ifndef GAMEINITIALIZER_H
#define GAMEINITIALIZER_H

class GameInitializer {
    private:
        GameController* gameController; // Liberado aca.
        ConnectionManager* connectionManager; // Liberado aca.
        GameControllerProxy* gameControllerProxy; // Liberado aca.

        Camera* camera; // Liberado en PitchView.
        Team* localTeam; // Liberado en Pitch.
        Team* awayTeam; // Liberado en Pitch.
        Pitch* pitch; // Liberado en game controller.
    public:
        GameInitializer(Conf* configuration);
        Pitch* getPitch();
        GameController* getGameController();
        Camera* getCamera();
        ConnectionManager* getConnectionManager();
        GameControllerProxy* getGameControllerProxy();
        ~GameInitializer();
    private:
        void initializePitch(Conf* conf);
        void initializeTeam(Conf* conf, int teamNumber);
        void initializeBall();
        void initializeGameController();
        void initializeConnectionManager(Conf* configuration);
        void initializeGameControllerProxy();
        std::string getTeamString(int teamNumber);
        void setTeam(Team* team, int teamNumber);
        int getOrientation(int teamNumber);
};
#endif // GAMEINITIALIZER_H
