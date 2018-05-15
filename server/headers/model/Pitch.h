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
        std::map<int, Team*> teams;
    public:
        Pitch(Camera* camera);
        Player* activePlayer;
        void changeActivePlayer(int user);
        void setTeam(Team* team, int teamNumber);
        void setBall(Ball* ball);
        std::list<Player*> getPlayersInsideCamera();
        Ball* getBall();
        void checkIntercepts();
        void changeBallOwnership();
        Team* getTeam(int user);
        ~Pitch();
};
#endif // PITCH_H
