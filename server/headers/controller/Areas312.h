#include "util/Constants.h"
#include "util/logger.h"
#include "controller/Areas.h"

#ifndef AREAS312_H
#define AREAS312_H

class Areas312 : public Areas {
    protected:
       virtual void loadAreas();
       virtual void loadDefendersAreas();
       virtual void loadMidFildersAreas();
       virtual void loadAtackersAreas();
    public:
        Areas312();
        ~Areas312();
};
#endif // AREAS312_H

