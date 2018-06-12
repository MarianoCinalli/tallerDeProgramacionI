#include "model/Pitch.h"
#include "controller/actions/Action.h"
#include "view/Camera.h"
#include "view/Clock.h"
#include "view/Score.h"

/*
Controlador de juego:
    - Tiene una referencia a la cancha.
    - Tiene una referencia al jugador seleccionado.

    execute()
    En caso de un cambio de jugador, le pide a la cancha la referencia
    a un jugador (mas cercano a otro jugador, a la pelota, lo que sea).
    Cuando se presione una tecla que se ejecute sobre un jugador el action
    se aplica sobre la referencia del jugador activo.

    Tambien al finalizar el procesamiento de teclas, updatePlayers() actualiza
    el estado de todos los jugadores del modelo; Aca actuaria el controlador
    de la cancha validando las posiciones de los jugadores.
*/

using namespace std;

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

class GameController {
    private:
        Pitch* pitch;
        Player* activePlayer;
        Ball* ball;
        Clock* clock;
        Score* score;
        map<int, Player*> players;

    public:
        GameController(Pitch* pitch);
        // Para actualizar los modelos.
        Player* getPlayer(int num);
        Ball* getBall();
        Camera* getCamera();
        Clock* getClock();
        Score* getScore();
        ~GameController();
};
#endif // GAMECONTROLLER_H
