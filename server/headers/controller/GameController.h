#include "model/Pitch.h"
#include "controller/actions/Action.h"
#include "view/Camera.h"
#include "util/Timer.h"
#include <set>

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
        Camera* camera;
        Pitch* pitch;
        Ball* ball;
        Player* activePlayer;
        Timer* timer;
        int state;
        int stateOption;
        bool end;
        int time;
        std::map<int,std::set<std::string>> users;
    public:
        GameController(Pitch* pitch, Camera* camera, Timer* timer);
        // Ejecuta la accion sobre el jugador seleccionado.
        void execute(Action* action, std::string user);
        // Para actualizar los modelos.
        void updatePlayers();
        void update();
        void checkState();
        void checkGoal();
        // Dependiendo de la posicion del active player, mueve la camara.
        void updateCameraPosition();
        // Dependiendo del estado del juego devuelve si termino o no.
        void updateBall();
        void addUser(std::string user, int teamNum, int formation);
        Player* getActivePlayer(std::string user);
        std::string getTeamStats(int numberTeam);
        bool joinTeam(std::string playerName, int team,int formation, int maxPlayers, std::string & errorMessage);
        void removeUser(std::string user);
        void startGame();
        bool hasGameStarted();
        bool shouldGameEnd();
        void setEnd();
        std::string getMessageToBroadcast(bool allPlayers);
        ~GameController();
    private:
        void removeUserFromTeam(std::string user);
        std::string getUsersWithTeamAsString();
        std::string getGameStatsMessage();
};
#endif // GAMECONTROLLER_H
