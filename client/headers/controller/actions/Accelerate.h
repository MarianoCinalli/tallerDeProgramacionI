#include "controller/actions/Action.h"
#include "util/Constants.h"
#include "model/Player.h"
#include "util/logger.h"

#ifndef ACCELERATE_H
#define ACCELERATE_H

class Accelerate : public Action {
    private:
        int direction;
    public:
        Accelerate(int direction);
        virtual std::string getCommand();
        virtual std::string getParameters();
        ~Accelerate();
};
#endif // ACCELERATE_H
