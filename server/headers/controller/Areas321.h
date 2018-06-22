#include "util/Constants.h"
#include "util/logger.h"
#include "controller/Areas.h"

#ifndef AREAS321_H
#define AREAS321_H

class Areas321 : public Areas {
    protected:
        virtual void loadAreas();
        virtual void loadDefendersAreas();
        virtual void loadMidFildersAreas();
        virtual void loadAtackersAreas();
    public:
        Areas321();
        ~Areas321();
};
#endif // AREAS321_H
