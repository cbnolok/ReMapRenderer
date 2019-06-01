//
// File: Renderer.h
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

#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "MapBlock.h"
#include "Texture.h"

#include <string>
#include <vector>

#include "SDL/SDL.h"
#include "Texture.h"

class Renderer
{
private:
	void RenderTerrain (SDL_Surface * target, int mx, int my);
	void RenderStatics (SDL_Surface * target, int mx, int my);
		
public:
   Renderer ();
   virtual ~Renderer ();
      
   void Rebuild (int mx, int my);
	
   INT_Rect CalcRenderBox();
   
protected:
   int hotspotx, hotspoty;
};

#endif //_CRenderer_H_
