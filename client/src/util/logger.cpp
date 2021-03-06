#include "util/logger.h"

extern int LOG_MIN_LEVEL;
extern std::ofstream LOG_FILE_POINTER;
extern std::mutex log_mutex;

std::string getFormatedDateTime(const char* format) {
    std::time_t now = std::time(nullptr);
    char buf[40];
    struct tm tstruct = *localtime(&now); // UTC
    // tstruct.tm_hour -= 3; // UTC - 3
    // Time might be out of range after fixing timezone. Re-converting it.
    std::mktime(&tstruct);
    strftime(buf, sizeof(buf), format, &tstruct);
    return buf;
}

std::string getDate() {
    return getFormatedDateTime("%F %T");
}

std::string getTime() {
    return getFormatedDateTime("%X");
}

std::string getPID() {
    return std::to_string(pthread_self());
}

std::string getMessageLevelString(int messageLevel) {
    switch(messageLevel) {
        case LOG_ERROR:
            return "ERROR";
            break;
        case LOG_INFO:
            return "INFO";
            break;
        case LOG_DEBUG:
            return "DEBUG";
            break;
        case LOG_SPAM:
            return "SPAM";
            break;
        default:
            return "WrongMessageLevel";
            break;
    }
}

int getLogLevelFromString(std::string level) {
    std::transform(level.begin(), level.end(), level.begin(), ::tolower);
    if (level == "error") {
        return LOG_ERROR;
    } else if (level == "info") {
        return LOG_INFO;
    } else if (level == "debug") {
        return LOG_DEBUG;
    } else if (level == "spam") {
        return LOG_SPAM;
    } else {
        return LOG_WRONGLEVEL;
    }
}

// PRE: Global variables LOG_MIN_LEVEL and LOG_FILE_POINTER have to be initialized.
// Returns 0 if the message was logged, 1 otherwise.
int log(std::string message, int messageLevel) {
    if (messageLevel < LOG_MIN_LEVEL) {
        return 1;
    }
    std::string toLog = getTime() + " - " + getPID() + " <" + getMessageLevelString(messageLevel) +"> " + message + "\n";
    log_mutex.lock();
    LOG_FILE_POINTER << toLog;
    flushLog();
    log_mutex.unlock();
    return 0;
}

// Logs a separator indicating the session start.
int logSessionStarted() {
    std::string separator = "----------------------------------------------------------------------------------------------------------\n";
    std::string sessionStartHeader = "Program started at " + getDate() + " by PID " + getPID() + "\n";
    LOG_FILE_POINTER << separator;
    LOG_FILE_POINTER << sessionStartHeader;
    LOG_FILE_POINTER << separator;
    flushLog();
    return 0;
}

// Logs a separator indicating the session finished.
int logSessionFinished() {
    std::string separator = "----------------------------------------------------------------------------------------------------------\n";
    std::string sessionFinishedHeader = "Program finished at " + getDate() + " by PID " + getPID() + "\n";
    LOG_FILE_POINTER << separator;
    LOG_FILE_POINTER << sessionFinishedHeader;
    flushLog();
    return 0;
}

void flushLog() {
    LOG_FILE_POINTER.flush();
}

// Overloaded loggers with concatenators ---------------------------------------------------------------------

int log(std::string messageToConcatenate, int integerToConcatenate, int messageLevel) {
    std::string intToString = std::to_string(integerToConcatenate);
    std::string concatenatedMessage = messageToConcatenate + intToString;
    return log(concatenatedMessage, messageLevel);
}

int log(std::string messageToConcatenate, std::string stringToConcatenate, int messageLevel) {
    std::string concatenatedMessage = messageToConcatenate + stringToConcatenate;
    return log(concatenatedMessage, messageLevel);
}

int log(std::string messageToConcatenate, const char* charsToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (charsToConcatenate != NULL) {
        std::string charToString(charsToConcatenate);
        concatenatedMessage = messageToConcatenate + charToString;
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** char* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}

int log(std::string messageToConcatenate, char* charsToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (charsToConcatenate != NULL) {
        std::string charToString(charsToConcatenate);
        concatenatedMessage = messageToConcatenate + charToString;
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** char* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}

int log(Loggable* classToConcatenate, int messageLevel) {
	std::string toLog;
    if (classToConcatenate != NULL) {
        toLog = classToConcatenate->toString();
    } else {
        toLog = "[ **error** classToConcatenate* is NULL]";
    }
    return log(toLog, messageLevel);
}

int log(std::string messageToConcatenate, Loggable* classToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (classToConcatenate != NULL) {
        concatenatedMessage = messageToConcatenate + classToConcatenate->toString();
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** classToConcatenate* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}
