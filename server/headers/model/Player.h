#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include "util/logger.h"
#include "controller/PlayerMovement.h"
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
        bool highPass;
        bool isSelected;
        bool isReturning;
        Velocity* velocity;
        Coordinates* position;
        Coordinates* basePosition;
        bool sliding;
        bool slided;
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
        PlayerMovement* playerMovement;
    public:
        static int ID;
        Player(Coordinates* position, int team);

        // Getters
        Coordinates* getPosition();
        Velocity* getVelocity();
        int getTeam();
        int getCurrentSpeed();
        int getOrientation();
        int getKickPower();
        int getId();
        bool isAHighPass();
        bool isThisPlayer(Player* player);
        bool slidedYet();
        bool hasKicked();
        bool isSliding();
        bool isKicking();
        bool getIsSelected();
        bool isRunningDiagonaly();
        bool isRunningFast();
        bool isGoalkeeper();
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
        void cantMoveUntilPass();
        void setCanMove(bool canMove);

        // Cambia la posicion con respecto a su velocidad.
        void updatePosition(Coordinates* positionToFollow);
        void updateKicking();
        void setKicked(bool);
        void setSlided(bool s);   //TODO mejorar nombres de slide!
        void updateSliding(Coordinates*);
        void updateState(Coordinates* positionToFollow);
        void setOrientation(int orientation);
        void setPosition(Coordinates coord);
        void setBasePosition(Coordinates coord);
        void setPosition(Coordinates* coord);
        void setBasePosition(Coordinates* coord);
        void returnToBasePosition();
        void setMovement(PlayerMovement* playerMovement);
        // Acciones
        void startsRunningFast();
        void stopsRunningFast();
        void startsSliding();
        void stopSliding();
        void startsKicking(int power, bool highPass);
        void stopKicking();
        void isAlreadyKicking();

        std::string getAsYaml();

        ~Player();
    private:
        void changeVelocityTo(Coordinates* positionToFollow, bool onlyX, bool onlyY);
};
#endif // PLAYER_H
