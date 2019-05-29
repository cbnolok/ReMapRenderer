//
// File: Map.h
// Created by:  Alexander Oster - tensor@ultima-iris.de
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

#ifndef _UOMAP_H_
#define _UOMAP_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "../uotype.h"
#include "Map.h"
#include "../Debug.h"
#include "MapPatches.h"


#define UOMAP_MAP0  1
#define UOMAP_MAP2  2

class UOMapLoader : public MapLoader
{
private:
	std::ifstream * mapstream;
   std::ifstream * staticstream;
   std::ifstream * staidxstream;
   int width, height;

   UOMapPatches * patches;


public:
   UOMapLoader (char * mapfile, char * staticfile, char * staidx, int type);
   virtual ~UOMapLoader ();

   void	LoadMapBlock( int x, int y, MulBlock *block );
   struct staticinfo * LoadStatics(int x, int y, int &len);


   unsigned int AddObject(unsigned int x, unsigned int y, int z, unsigned int tileid);
   unsigned int DelObject(unsigned int blockx, unsigned int blocky, unsigned int obj_id);

   void Save(char * filename);
   void Load(char * filename);
   

};


#endif //_MAP_H_
