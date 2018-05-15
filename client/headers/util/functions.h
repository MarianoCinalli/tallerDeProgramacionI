#ifndef CONNECTION_FUNCTIONS_H
#define CONNECTION_FUNCTIONS_H

#include <unistd.h> // para el usleep.
#include <string>
#include <yaml-cpp/yaml.h>
#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/ConnectionManager.h"

void* read_server(void* argument);
void* drawer(void* clients);
#endif
