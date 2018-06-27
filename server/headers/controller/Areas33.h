#include "util/Constants.h"
#include "util/logger.h"
#include "controller/Areas.h"

#ifndef AREAS33_H
#define AREAS33_H

class Areas33 : public Areas {
    protected:
        virtual void loadAreas();
        virtual void loadDefendersAreas();
        virtual void loadMidFildersAreas();
        virtual void loadAtackersAreas();
    public:
        Areas33();
        ~Areas33();
};
#endif // AREAS33_H

