//
// File: Animation.h
// Created by: Alexander Oster - tensor@ultima-iris.de
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

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <vector>

struct sAnimationFrame
{
    SDL_Surface * surface;
    int centerx, centery;
};

class cAnimation
{
  private:
    std::vector <sAnimationFrame *> frames;
  
  public:
    cAnimation ();
    ~cAnimation ();
    
    void ClearFrames();
    sAnimationFrame * AddFrame();
    sAnimationFrame * GetFrame(Uint32 frame);
    Uint32 count () { return frames.size(); }
	
  protected:

};


#endif //_ANIMATION_H_
