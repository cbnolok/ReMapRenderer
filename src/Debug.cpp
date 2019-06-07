//
// File: Debug.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
// Modified by: DarkStorm (4.3.2003)
//

#include <iostream>
#include "Debug.h"


Debug pDebug;

using namespace std;

Debug::Debug()
{
    loglevel = LEVEL_WARNING;
}

Debug::~Debug()
{
}

// Only print to stderr in Debug mode, otherwise redirect to file (debug.txt)
#ifndef _DEBUG
bool logToFile = true;
#else
bool logToFile = false;
#endif

void Debug::Log(const char *message, char *filename, int line, int level)
{
    // Ignore messages below our loglevel
    if (level < loglevel)
        return;

    std::string errorMsg;
    errorMsg += "(";
    errorMsg += filename;
    errorMsg += ",";
    errorMsg += std::to_string(line);
    errorMsg += "): ";
    errorMsg += message;

    Log(errorMsg.c_str(), level);
}

void Debug::Log(const char *message, int level)
{
    // Ignore messages below our loglevel
    if (level < loglevel)
        return;

    std::string errorMsg;

    switch (level) {
        case LEVEL_WARNING:
            errorMsg = "[Warning] ";
            break;
        case LEVEL_ERROR:
            errorMsg = "[Error] ";
            break;
        case LEVEL_CRITICAL:
            errorMsg = "[CRITICAL Error] ";
            break;
    }

    errorMsg += message;
    errorMsg += "\n";

    // Dump the message out to a file
    if (logToFile) {
        FILE *fp = fopen("debug.txt", "at");

        if (fp) {
            fprintf(fp, "%s\n", message);
            fclose(fp);
        }
    }
    else {
        fprintf(stderr, "%s\n", message);
    }
}

int Debug::GetLoglevel(void)
{
    return loglevel;
}

void Debug::SetLoglevel(int loglevel)
{
    this->loglevel = loglevel;
}
