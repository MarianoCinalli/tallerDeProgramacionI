#include <string>

#ifndef LOGGABLE_H
#define LOGGABLE_H

class Loggable {
    public:
        Loggable(){}
        virtual ~Loggable(){}
        virtual std::string toString()=0;
};
#endif // LOGGABLE_H
