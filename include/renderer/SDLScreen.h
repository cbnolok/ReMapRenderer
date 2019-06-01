//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
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

#ifndef _SDLSCREEN_H_
#define _SDLSCREEN_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include "SDL/SDL.h"
#include <map>
#include <list>

using namespace std;

class SDLScreen
{
private:
  const SDL_VideoInfo *videoInfo;
  Uint8 act_alpha;
  
  float light_factor;
  Uint32 act_update;

public:
    SDLScreen ();
   ~SDLScreen ();

  int Init (int width, int height, int bpp);
  SDL_Surface *screen;
  int videoFlags;
  
  void Save (char * filename) {
    SDL_SaveBMP(screen, filename);
  }
};

extern float BilinearTable[32][32][4];

#endif //_SDLSCREEN_H_
