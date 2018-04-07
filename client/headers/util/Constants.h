#ifndef CONSTANTS_H_
#define CONSTANTS_H_

enum logLevels {
    LOG_DEBUG = 1,
    LOG_INFO = 2,
    LOG_ERROR = 3
};

// Para indicar a donde se mueve/mira el jugador.
enum PlayerOrientations {
    PLAYER_ORIENTATION_LEFT = 1,
    PLAYER_ORIENTATION_UP = 2,
    PLAYER_ORIENTATION_RIGHT = 3,
    PLAYER_ORIENTATION_DOWN = 4
};

//The dimensions of the level
const int LEVEL_WIDTH = 1600;
const int LEVEL_HEIGHT = 1000;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int MARGIN_SIZE = 80;

#endif // CONSTANTS_H_
