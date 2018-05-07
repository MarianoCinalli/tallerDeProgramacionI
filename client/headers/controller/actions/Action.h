#include "model/Player.h"
#include "model/Pitch.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef ACTION_H
#define ACTION_H

class Action {
    private:
    public:
        Action();
        virtual void execute(Player* player);
        virtual void execute(Pitch* pitch);
        virtual bool valid(Player* player);
        virtual bool valid(Pitch* pitch);
        virtual std::string getCommand();
        virtual std::string toString();
        virtual ~Action();
};
#endif // ACTION_H
