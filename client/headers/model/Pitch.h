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


    public:
        Camera* camera;
        Pitch(Camera* camera);
        Player* activePlayer;
        Ball* ball;
        void setTeam(Team* team, int teamNumber);
        Team* getTeam(int teamNumber);
        void setBall(Ball* ball);
        ~Pitch();
};
#endif // PITCH_H
