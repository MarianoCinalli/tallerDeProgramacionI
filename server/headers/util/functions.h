#ifndef CONNECTION_FUNCTIONS_H
#define CONNECTION_FUNCTIONS_H

#include <string>
#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/Broadcaster.h"
#include "util/ConnectionManager.h"
#include "util/User.h"
#include "view/Camera.h"
#include "controller/GameControllerProxy.h"
#include <map>
#include <pthread.h>
#include <signal.h>

static const int MICROSECONDS_BETWEEEN_BROADCAST = 1000000/120;

void* read_client(void* argument);
void* broadcast_to_clients(void* clients);
void* game_updater(void* argument);

std::string getMessageKey(std::string message);
std::string getMessageValue(std::string message);

void* connection_listener(void* argument);

// Signal handling ---------------------------------------------------------------------------
void handleSignal(int sig);
void registerSignalHandler(int signal);
void setQuit();

#endif
