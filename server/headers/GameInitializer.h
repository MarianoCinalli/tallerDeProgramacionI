#include <string>
#include <map>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "util/ConnectionManager.h"
#include "util/Coordinates.h"
#include "util/Colour.h"
#include "util/UsersManager.h"
#include "util/Timer.h"
#include "model/Player.h"
#include "model/Pitch.h"
#include "model/Ball.h"
#include "model/Team.h"
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
        UsersManager* usersManager; // Liberado aca.
        Timer* timer; // Liberado aca.

        Camera* camera; // Liberado en PitchView.
        Team* localTeam; // Liberado en Pitch.
        Team* awayTeam; // Liberado en Pitch.
        Pitch* pitch; // Liberado en game controller.
        Conf* configuration;
    public:
        GameInitializer(Conf* configuration);
        Pitch* getPitch();
        GameController* getGameController();
        Camera* getCamera();
        ConnectionManager* getConnectionManager();
        GameControllerProxy* getGameControllerProxy();
        Timer* getTimer();
        ~GameInitializer();
        Conf* getConfiguration();
        UsersManager* getUsersManager();
    private:
        void initializePitch(Conf* conf);
        void initializeTeam(Conf* conf, int teamNumber);
        void initializeBall(Conf* conf);
        void initializeGameController();
        void initializeConnectionManager(Conf* configuration);
        void initializeGameControllerProxy();
        void initializeUsersManager(Conf* configuration);
        void initializeTimer(Conf* configuration);
        std::string getTeamString(int teamNumber);
        void setTeam(Team* team, int teamNumber);
};
#endif // GAMEINITIALIZER_H
