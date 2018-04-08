#include <list>
#include "model/Player.h"

#ifndef TEAM_H
#define TEAM_H

class Team {
    private:
        std::list<Player*> players;
        int formacion;

    public:
        Team();
        ~Team();
        void order();
        void addPlayer(Player* player);
        std::list<Player*> getPlayers();
        void setFormacion(int formacion);
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
