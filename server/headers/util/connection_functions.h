#ifndef CONNECTION_FUNCTIONS_H
#define CONNECTION_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>


void* read_client(void* argument);

#endif
