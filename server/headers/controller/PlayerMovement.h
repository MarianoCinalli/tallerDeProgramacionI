#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

class PlayerMovement {
    private:
        int defenders;
        int midfielders;
        int attackers;
    public:
        PlayerMovement();
        void setFormation(int formation);
        bool canMoveTo(int x, int y, int playerNumber, bool isSelected);
        ~PlayerMovement();
};
#endif // PLAYERMOVEMENT_H
