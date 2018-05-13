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
#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"

void* read_server(void* argument);
void* drawer(void* clients);
#endif
