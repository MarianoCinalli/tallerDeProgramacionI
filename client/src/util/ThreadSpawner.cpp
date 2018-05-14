#include "util/ThreadSpawner.h"

ThreadSpawner::ThreadSpawner() {
    this->ids = {};
}

bool ThreadSpawner::spawn(void* (*routine) (void*), void* arg) {
    pthread_t threadId;
    int status = pthread_create(&threadId, NULL, routine, arg);
    // De man pthread_create:
    // On success, pthread_create() returns 0; on error, it 
    // returns an error number, and the contents of *thread are undefined.
    if (status != 0) {
        log("ThreadSpawner: No se pudo crear el thread! Codigo de error: ", status, LOG_ERROR);
        return false;
    } else {
        log("ThreadSpawner: Thread creado con id: ", threadId, LOG_INFO);
        this->ids.push_back(threadId);
        return true;
    }
}

void ThreadSpawner::joinSpawnedThreads() {
    log("ThreadSpawner: Esperando a que los threads terminen...", LOG_INFO);
    for(pthread_t threadId : this->ids) {
        log("ThreadSpawner: Esperando al thread de ID: ", threadId, LOG_INFO);
        int status = pthread_join(threadId, NULL);
        if (status != 0) {
            log("ThreadSpawner: No se pudo joinear el thread! Codigo de error: ", status, LOG_ERROR);
        } else {
            log("ThreadSpawner: Termino el thread de ID: ", threadId, LOG_INFO);
        }
    }
    log("ThreadSpawner: Los threads terminaron.", LOG_INFO);
}

ThreadSpawner::~ThreadSpawner() {
}


