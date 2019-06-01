//
// File: MapBlock.h
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

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <iostream>
#include "StaticObjects.h"
#include "Node.h"

struct INT_Rect {
    int x, y, w, h;
};

class cMapblock
{
private:
 Sint8 heightmap[10][10];

 unsigned short groundids[8][8];
 int ground_draw_pos[10][10][2];
 SDL_Surface * custom_stretched[8][8];
 int hotspots[8][8][2];
  float alpha_values[9][9];
 
 SDL_Rect outbox;

 StaticObjectList objects;
 
public:
    cMapblock ();
   ~cMapblock ();
   bool Generate(int blockx, int blocky);
   void RenderGround(int x, int y, SDL_Surface * target, SDL_Rect * cliprect = NULL);
   void RenderStatics(int x, int y, SDL_Surface * target, SDL_Rect * cliprect = NULL, int tile_type = TILE_CLASS_ALL);
	   
   int blockx, blocky;
   
   void GetBounds(INT_Rect * rect, int x, int y);
protected:

};


#endif //_MAPBLOCK_H_
