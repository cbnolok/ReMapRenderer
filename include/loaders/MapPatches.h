//
// File: MapPatches.h
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

#ifndef _MAPPATCHES_H_
#define _MAPPATCHES_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include "../uotype.h"


class UOMapPatches
{
private:

public:

   virtual void AddPatch(struct StaticPatch patch) = 0;
   virtual void DelPatch(unsigned int blockx, unsigned int blocky, unsigned int obj_id) = 0;
   virtual int GetNewID(int blockx, int blocky) = 0;
   
   virtual int GetNewStaticSize(int orig_count, unsigned int blockx, unsigned int blocky, bool &apply_needed) = 0;
   virtual void ApplyPatch(unsigned int blockx, unsigned int blocky, unsigned int oldcount, unsigned int newcount, struct staticinfo * src, struct staticinfo * dst) = 0;

};


#endif //_MAP_H_
