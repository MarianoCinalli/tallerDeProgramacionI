#include <list>
#include <string>
#include "model/Player.h"

#ifndef TEAM_H
#define TEAM_H


class Team {
    private:
        std::list<Player*> players;
        int local; // 0 si es local, 1 si es visitante
        int formacion;
        std::string name;
        int score;
    public:
        Team(int, std::string);
        ~Team();
        void order();
        void addPlayer(Player* player);
        std::list<Player*> getPlayers();
        Player* getPlayer(int num);
        void setFormacion(int formacion);
        int getTeamNum();
        int getAmountPlayers();
        std::string getName();
        void increaseScore();
        int getScore();
};
#endif // TEAM_H

// Para iterar sobre una lista de punteros a Player.
// for (std::list<Player*>::iterator it = list.begin(); it != list.end(); it++) {
//     std::cout << (*it)->getOrientation() << std::endl;
// }

//otra manera
//for (Player* p : list){
//    p->getOrientation();
//}
