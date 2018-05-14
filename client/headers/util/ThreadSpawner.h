#include <pthread.h>
#include <vector>
#include "util/Constants.h"
#include "util/logger.h"

#ifndef THREADSPAWNER_H
#define THREADSPAWNER_H

class ThreadSpawner {
    private:
        std::vector<pthread_t> ids;
    public:
        ThreadSpawner();
        bool spawn(void* (*routine) (void*), void* arg);
        void joinSpawnedThreads();
        ~ThreadSpawner();
};
#endif // THREADSPAWNER_H


