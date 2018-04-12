#ifndef CONSTANTS_H_
#define CONSTANTS_H_

enum logLevels {
    LOG_DEBUG = 1,
    LOG_INFO = 2,
    LOG_ERROR = 3
};

// Para indicar a donde se mueve/mira el jugador.
enum PlayerOrientations {
    PLAYER_ORIENTATION_UP = 1,
    PLAYER_ORIENTATION_DOWN = 2,
    PLAYER_ORIENTATION_LEFT = 3,
    PLAYER_ORIENTATION_RIGHT = 4
};

//para indicar accion a realizar
enum PlayerStandedActions {
    STANDED_PLAYER = 0,
    SLIDE_ACTION = 5,
    KICK_ACTION = 6,
};

const int SALIDA_LINEA_COMANDOS = 4;

//The dimensions of the level
const int LEVEL_WIDTH = 1600;
const int LEVEL_HEIGHT = 1000;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int MARGIN_SIZE = 80;
const int SPRITE_SIZE = 16;


#endif // CONSTANTS_H_
