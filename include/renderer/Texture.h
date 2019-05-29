//
// File: Texture.h
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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include "../Buffer.h"

class Texture : public BufferEntry
{
private:

  SDL_Surface * surface;

public:
    Texture ();
   ~Texture ();

   int LoadFromData( void * data, int width, int height, int bits_per_pixel);
   int LoadFromData16( void * data, int width, int height, int bits_per_pixel);
   int LoadFromFile( const char * filename);
   int Create16(int width, int height);

   int GetWidth ();
   int GetHeight ();
   
   SDL_Surface * GetSurface () { return surface; }
   
protected:

};


#endif //_TEXTURE_H_
