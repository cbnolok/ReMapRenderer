//
// File: Debug.h
//

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <string>

#define LEVEL_WARNING  0
#define LEVEL_ERROR    1
#define LEVEL_CRITICAL 2
#define LEVEL_NONE     3

class Debug
{
private:
    int loglevel;

public:
    Debug ();
    ~Debug ();

    void Log(const char * message, char * filename, int line, int level);
    void Log(const char * message);
    void Log(std::string const& message) { Log (message.c_str()); }
    int GetLoglevel(void);
    void SetLoglevel(int loglevel);
};


extern	Debug	pDebug;

#endif //_DEBUG_H_
