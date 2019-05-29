//
// File: LocalMapPatches.h
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

#ifndef _LOCALMAPPATCHES_H_
#define _LOCALMAPPATCHES_H_

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


struct PatchEntry {
	struct StaticPatch patch;
	struct PatchEntry * next;
} STRUCT_PACKED;



class LocalMapPatches : public UOMapPatches
{
private:
	struct PatchEntry * root;
	int count;


public:

   LocalMapPatches ();
   virtual ~LocalMapPatches ();

   void Clear();
   void AddPatch(struct StaticPatch patch);
   void DelPatch(unsigned int blockx, unsigned int blocky, unsigned int obj_id);
   int GetNewID(int blockx, int blocky);

   StaticPatch * GetPatch(unsigned int blockx, unsigned int blocky, unsigned int objid);
   int GetNewStaticSize(int orig_count, unsigned int blockx, unsigned int blocky, bool &apply_needed);

   void ApplyPatch(unsigned int blockx, unsigned int blocky, unsigned int oldcount, unsigned int newcount, struct staticinfo * src, struct staticinfo * dst);

   int Load (char * filename);
   int Save (char * filename);



};


#endif //_MAP_H_
