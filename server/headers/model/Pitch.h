#include "model/Team.h"
#include "view/Camera.h"
#include "model/Ball.h"
#include <map>

/*
Modelo de Cancha:
    Tiene el equipo con referencias a los 7 jugadores del modelo.
    Sabe que jugador devolver cuando se cambia el activo.
*/

#ifndef PITCH_H
#define PITCH_H

class Pitch {
    private:
        Team* localTeam;
        Team* awayTeam;
        Camera* camera;
        Ball* ball;
        int initialSides;
        std::map<Team*,std::list<std::string>> teams;
        std::map<std::string, Player*> activePlayers;
    public:
        Pitch(Camera* camera);
        // Player* activePlayer;
        void setActivePlayer(std::string user, Player* player);
        void changeActivePlayer(std::string user);
        Player* getActivePlayer(std::string user);
        void setTeam(Team* team, int teamNumber);
        void setUserTeam(std::string user, int team, int formation);
        bool setTeamFormation(int teamNum, int formation);
        void setBall(Ball* ball);
        std::list<Player*> getPlayersInsideCamera();
        std::list<Player*> getPlayers();
        Ball* getBall();
        Camera* getCamera();
        int goalkick();
        Player* setStart(int position);
        void checkSteals();
        void changeBallOwnership();
        void removeActivePlayer(std::string user);
        Team* getTeam(int teamNumber);
        Team* getTeamBySide(int teamNum);
        std::string getScoresYAML();
        std::string getDebugLines();
        void changeSides();
        ~Pitch();
    private:
        std::string getUsersWithActivePlayersAsString();
};
#endif // PITCH_H
