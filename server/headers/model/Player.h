#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include "contoller/PlayerMovent.h"
#include <SDL2/SDL.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
    private:
        int id;
        int team;
        int orientation;
        int maxVelocity;
        int kickPower;
        bool isSelected;
        bool isReturning;
        Velocity* velocity;
        Coordinates* position;
        Coordinates* basePosition;
        bool sliding;
        bool wasSliding;
        bool kicking;
        bool kicked;
        bool canMove;
        bool runningFast;
        bool withBall;
        int kickCount;
        int slideCount;
        std::string userName;
        int fieldPosition;
        int stealCoef;
        PlayerMovent* playerMovent;
    public:
        static int ID;
        Player(Coordinates* position, int team, PlayerMovent* playerMovent);

        // Getters
        Coordinates* getPosition();
        Velocity* getVelocity();
        int getTeam();
        int getCurrentSpeed();
        int getOrientation();
        int getKickPower();
        bool wasSlidingYet();
        bool hasKicked();
        bool isSliding();
        bool isKicking();
        bool getIsSelected();
        bool isRunningDiagonaly();
        bool isRunningFast();
        bool isStill();
        std::string getUsername();
        int getStealCoef();
        // Modifiers
        // Cambia la velocidad a su maximo, por ahora.
        void accelerate(int direction);
        void decelerate(int direction);
        void stop();
        void stopRunningInDirection(int direction);
        void toggleIsSelected(std::string name);
        void copyStats(Player* copyTo);
        void setFieldPosition(int formation);
        void setTrayectory(Velocity* trayectory);
        void setWithBall(bool dominated);
        bool isWithBall();
        // Cambia la posicion con respecto a su velocidad.
        void updatePosition(Coordinates* positionToFollow);
        void updateKicking();
        void setKicked(bool);
        void updateSliding();
        void updateState(Coordinates* positionToFollow);
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);
        void setBasePosition(Coordinates coord);
        void returnToBasePosition();
        // Acciones
        void startsRunningFast();
        void stopsRunningFast();
        void startsSliding();
        void stopSliding();
        void isAlreadySliding();   //TODO mejorar nombres de slide!
        void startsKicking(int power);
        void stopKicking();
        void isAlreadyKicking();

        std::string getAsYaml();

        ~Player();
    private:
        void follow(Coordinates* positionToFollow);
};
#endif // PLAYER_H
