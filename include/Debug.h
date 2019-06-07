//
// File: Debug.h
//

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <string>

#define LEVEL_MSG       0
#define LEVEL_WARNING   1
#define LEVEL_ERROR     2
#define LEVEL_CRITICAL  3
#define LEVEL_NONE      4

class Debug
{
private:
    int loglevel;

public:
    Debug ();
    ~Debug ();

    void Log(const char * message, char * filename, int line, int level);
    void Log(const char * message, int level = LEVEL_MSG);
    void Log(std::string const& message) { Log (message.c_str()); }
    int GetLoglevel(void);
    void SetLoglevel(int loglevel);
};


extern	Debug	pDebug;

#endif //_DEBUG_H_
