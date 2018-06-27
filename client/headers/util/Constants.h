#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// No tocar que revienta una validacion
enum logLevels {
    LOG_SPAM = 1,
    LOG_DEBUG = 2,
    LOG_INFO = 3,
    LOG_ERROR = 4,
    LOG_WRONGLEVEL = 5
};

// Para indicar a donde se mueve/mira el jugador.
// No cambiar porque se rompen los sprites de barrer y patear.
enum PlayerOrientations {
    PLAYER_ORIENTATION_UP = 1,
    PLAYER_ORIENTATION_RIGHT = 2,
    PLAYER_ORIENTATION_DOWN = 3,
    PLAYER_ORIENTATION_LEFT = 4,
    PLAYER_ORIENTATION_INVALID = 5
};

enum states {
  NORMAL_STATE = 1,
  HALF_START_STATE = 2,
  GOAL_STATE = 3,
  THROWIN_STATE = 4,
  GOALKICK_STATE = 5,
  GAME_START_STATE = 6,
  GAME_END_STATE = 7
};

//state vars
const int CENTER_LEFT_START = 0;
const int CENTER_RIGHT_START = 1;
const int LEFT_START = 2;
const int RIGHT_START = 3;


enum KeyboardBools {
    KUP = 0,
    KDOWN = 1,
    KLEFT = 2,
    KRIGHT = 3
};

//para indicar accion a realizar
enum PlayerStandedActions {
    STANDED_PLAYER = 0,
    SLIDE_ACTION = 5,
    KICK_ACTION = 6,
};

const int SALIDA_LINEA_COMANDOS = 4;

// The dimensions of the level
const int LEVEL_WIDTH = 1600;
const int LEVEL_HEIGHT = 1000;

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 700;

// const int SCREEN_HEIGHT = 700;

// Necesario para la camara
const int SPRITE_SIZE = 63;
const int CUERPO_JUGADOR = 15;

const int DRAW_SIZE_PLAYER = 85;
const int DRAW_SIZE_CORRECTION = 15;


const int FIELD_CORRECTION = 0;
const int FIELD_WIDTH = 800*0.8;
const int FIELD_HEIGHT = 600*0.8;

// Velocidad de desplazamiento scroll camara
const int SCROLL_SPEED = 13;
const int CAMERA_OFFSET = 100;

const int VALOR_INVALIDO = 5;
const int ARCHIVO_INVALIDO = 6;
const int CASACA_PRINCIPAL = 0;
const int CASACA_ALTERNATIVA = 1;
const int PLAYERS_PER_TEAM = 7;

//Velocidad
const int NORMAL_VELOCITY = 15;
const int BALL_DECELERATE_TIME = 15; //a numeros mas grandes, tarda mas
const int BALL_DECELERATE_CONST = 3;

//interceptar pases
const int INTERCEPT_VALUE = 40;
const int STEAL_VALUE = 60;
const int TIME_BALL_NO_INTERCEPT = 4;

//render
const int DRAW_FRAME_RATE = 50;
// const int MILISECONDS_TIMEOUT = 20;
const int STANDING_DIVISOR = 10;
const int RUNNING_DIVISOR = 10;
const int SLIDING_DIVISOR = 7;
const int KICKING_DIVISOR = 6;
const int ROLLING_DIVISOR = 5;
const int ACTIVE_PLAYER_DIVISOR = 10;



#endif // CONSTANTS_H_
