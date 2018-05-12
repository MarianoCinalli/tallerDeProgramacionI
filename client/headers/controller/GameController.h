#include "model/Pitch.h"
#include "controller/actions/Action.h"
#include "view/Camera.h"

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

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

class GameController {
    private:
        Pitch* pitch;
        Player* activePlayer;
        Ball* ball;
    public:
        GameController(Pitch* pitch);
        // Ejecuta la accion sobre el jugador seleccionado.
        void execute(Action* action);
        // Para actualizar los modelos.
        void updatePlayers();
        void updateBall();
        // Dependiendo de la posicion del active player, mueve la camara.
        void updateCameraPosition(Camera* camera);
        ~GameController();
};
#endif // GAMECONTROLLER_H
