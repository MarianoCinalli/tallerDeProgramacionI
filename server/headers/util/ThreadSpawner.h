#include <pthread.h>
#include <signal.h>
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
        pthread_t spawn(void* (*routine) (void*), void* arg);
        void joinSpawnedThreads();
        void join(pthread_t id);
        void signal(pthread_t id, int signal);
        ~ThreadSpawner();
};
#endif // THREADSPAWNER_H


