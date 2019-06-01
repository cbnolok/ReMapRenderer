//
// File: Debug.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
// Modified by: DarkStorm (4.3.2003)
//

#include <iostream>
#include <string>
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
  if(level < loglevel)
    return;

  std::string errorMsg;

  switch (level) {
  case LEVEL_WARNING:
    errorMsg = "Warning ";
    break;
  case LEVEL_ERROR:
    errorMsg = "Error ";
    break;
  case LEVEL_CRITICAL:
    errorMsg = "CRITICAL Error ";
    break;
  }

  char lineStr[32];

  sprintf(lineStr, "%d", line);

  errorMsg += "(";
  errorMsg += filename;
  errorMsg += ",";
  errorMsg += lineStr;
  errorMsg += "): ";
  errorMsg += message;
  errorMsg += "\n";

  // Dump the message out to a file
  if(logToFile) {
    FILE *fp = fopen("debug.txt", "at");

    if(fp) {
      fprintf(fp, errorMsg.c_str());
      fclose(fp);
    }
  } else {
    fprintf(stderr, errorMsg.c_str());
  }
}

void Debug::Log(const char *message)
{

  // Dump the message out to a file
  if(logToFile) {
    FILE *fp = fopen("debug.txt", "at");

    if(fp) {
      fprintf(fp, "%s\n", message);
      fclose(fp);
    }
  } else {
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
