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

#ifdef WIN32
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
  void DisplayFps ();
  const SDL_VideoInfo *videoInfo;
  Uint8 act_alpha;
  
  float light_factor;
  Uint32 act_update;

public:
    SDLScreen ();
   ~SDLScreen ();

  int Init (int width, int height, int bpp);
  int ResizeWindow (int width, int height);
  void ToggleFullScreen ();
  void ClearScreen ();
  void ClearZBuffer ();
  int DrawGL (void);
  int ScreenSave ();
  SDL_Surface *screen;
  int videoFlags;
  void SetPerspective(void);
  float GetRatio(void);
  
  void SetAlpha(Uint8 alpha);
  void SetHue(Uint16 hue = 0);
  
  void ClearFonts ();
  void RegisterFont (Uint32 id, std::string filename, Uint32 size);
  void UnregisterFont (Uint32 id);
  int GetTextWidth(char * text, int font);
  
  void SetLight(float factor);
  
  void Flip();
  void DrawSurface (SDL_Surface * surface, int x, int y);
  
  void Lock();
  void Unlock();
  
  bool do_redraw;
  void CheckUpdate();
  
  std::list<Uint32> update_times;
  void DoUpdate (Uint32 time);
  
  void Save (char * filename) {
    SDL_SaveBMP(screen, filename);
  }
  
  
protected:

};

extern float BilinearTable[32][32][4];
extern void CalcBilinearTable ();

#endif //_SDLSCREEN_H_
