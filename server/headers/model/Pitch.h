#include "model/Team.h"
#include "view/Camera.h"
#include "model/Ball.h"

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
    public:
        Pitch(Camera* camera);
        Player* activePlayer;
        void changeActivePlayer();
        void setTeam(Team* team, int teamNumber);
        void setBall(Ball* ball);

        Team* getLocalTeam();
        std::list<Player*> getPlayersInsideCamera();
        Ball* getBall();
        ~Pitch();
};
#endif // PITCH_H
