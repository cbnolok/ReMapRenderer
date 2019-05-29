//
// File: MapBuffer.h
// Created by: Darkstorm
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

#ifndef _MAPBUFFER_H_
#define _MAPBUFFER_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "../include.h"
#include "MapBlock.h"
#include "SDL/SDL.h"
#include <map>
#include <vector>


class cMapbuffer 
{
private:
	typedef std::map<Uint32, cMapblock *>	MapBuffer_t;
 MapBuffer_t	root;
 std::vector<cMapblock *>  cache;

public:
    cMapbuffer ();
   ~cMapbuffer ();

   void Clear();
   int GetCount(void);

   cMapblock * Get (int x, int y);
   void Add(cMapblock * block);
   cMapblock * CreateBlock (int x, int y);
   void FreeBuffer(int blockx, int blocky, int radius);

protected:

};

extern	cMapbuffer	* pMapbuffer;

#endif //_MAPBLOCK_H_
