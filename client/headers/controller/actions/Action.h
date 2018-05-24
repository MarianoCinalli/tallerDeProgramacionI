#include "model/Player.h"
#include "model/Pitch.h"
#include "util/Constants.h"
#include "util/logger.h"
#include <vector>
#include <yaml-cpp/yaml.h>

#ifndef ACTION_H
#define ACTION_H

class Action {
    private:
    public:
        Action();
        virtual std::string getCommand();
        virtual std::string getParameters();
        virtual std::string toString();
        virtual ~Action();
};
#endif // ACTION_H
