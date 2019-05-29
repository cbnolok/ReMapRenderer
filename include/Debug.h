//
// File: Debug.h
// Created by:
//
/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
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

   void Log(char * message, char * filename, int line, int level);
   void Log(char * message);
   void Log(std::string message) { Log ((char *) message.c_str()); }
   int GetLoglevel(void);
   void SetLoglevel(int loglevel);

protected:

};


extern	Debug	pDebug;

#endif //_DEBUG_H_
