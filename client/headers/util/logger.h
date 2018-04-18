#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <algorithm>
#include <string>
#include "interfaces/Loggable.h"
#include "util/Constants.h"

// File write
#include <fstream>

// Time
#include <ctime>
#include <unistd.h>

// Funciones de uso interno.
std::string getFormatedDateTime(const char* format);
std::string getDate();
std::string getTime();
std::string getPID();
std::string getMessageLevelString(int messageLevel);
int getLogLevelFromString(std::string level);

// Logea
int log(std::string message, int messageLevel);

// Funciones sobrecargadas.
int log(std::string messageToConcatenate, int integerToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, std::string stringToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, const char* charsToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, char* charsToConcatenate, int messageLevel);
int log(Loggable* classToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, Loggable* classToConcatenate, int messageLevel);

// Funciones para el encabezado y pie de archivos.
int logSessionStarted();
int logSessionFinished();

#endif // LOGGER_H
