#include "model/Team.h"
#include "view/Camera.h"

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
    public:
        Pitch(Camera* camera);
        Player* activePlayer;
        void changeActivePlayer();
        void setLocalTeam(Team* team);
        void setAwayTeam(Team* team);
        Team* getLocalTeam();
        ~Pitch();
};
#endif // PITCH_H
