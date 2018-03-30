#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "util/Constants.h"
#include "util/logger.h"
#include "controller/cancha_controller.h"

// Global variables ---------------------------------------
int LOG_MIN_LEVEL = LOG_ERROR; // Cambiar por parametro parseado.
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
// Global variables ---------------------------------------

int main(int argc, char* argv[]) {
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
    
    // Program
    startView();

    logSessionFinished();
    LOG_FILE_POINTER.close();

    return 0;
}
