//
// File: AnimBuffer.h
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

#ifndef _ANIMBUFFER_H_
#define _ANIMBUFFER_H_

#include "SDL/SDL.h"
#include "Animation.h"
#include "../Cache.h"
#include "../Buffer.h"

class AnimBuffer  
{
public:
   AnimBuffer ();
   virtual ~AnimBuffer ();

   cAnimation * GetAnimation(int index);

protected:
	Cache< cAnimation > animations;
};

extern	AnimBuffer	* pAnimBuffer;

#endif //_ANIMBUFFER_H_
