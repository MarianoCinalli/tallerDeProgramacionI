#ifndef CONNECTION_FUNCTIONS_H
#define CONNECTION_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // para el usleep.
#include <string>
#include <string.h>
#include <errno.h>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/Broadcaster.h"

static const int MICROSECONDS_BETWEEEN_BROADCAST = 100000;

void* read_client(void* argument);
void* broadcast_to_clients(void* clients);
#endif
