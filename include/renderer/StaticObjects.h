//
// File: StaticObjects.h
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

#ifndef _STATICOBJECTS_H_
#define _STATICOBJECTS_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <list>

#include "Characters.h"

#define TILE_CLASS_GROUND    1
#define TILE_CLASS_ITEM      2
#define TILE_CLASS_CHARACTER 4

#define TILE_CLASS_ALL       15

class cEntity {
public:
  int tileclass;
  int x, y, z;
  int blockx, blocky;
  int w, h;
  int draw_x, draw_y;
  int tileid;
  int hue;
  unsigned int flags;
  unsigned int tiledata_flags;
  Uint32 id;
  int height;
  virtual ~cEntity () { };
};

class cStaticObject : public cEntity
{
public:
  cStaticObject () {
    id = 0;
  }
  virtual ~cStaticObject () { };
};

class cGroundObject : public cEntity
{
public:
  int stretch;
  cGroundObject ();
  virtual ~cGroundObject ();
};

class cStaticCharacter : public cEntity
{
public:
  Uint32 id;
  Uint16 model;
  cCharacter * character;
  
  cStaticCharacter ()
  {
    tileclass = TILE_CLASS_CHARACTER;
    id = 0;
    model = 0;
  }
};

typedef std::list<class cEntity *>  objectlist_t;

class StaticObjectList
{
private:
 objectlist_t objectlist;
public:
    StaticObjectList ()
    { }
    
   ~StaticObjectList ()
   {
   	Clear();
   }

   void Clear (void);
   
   void InsertDynamic (cStaticObject * object);
   void InsertCharacter (cStaticCharacter * object);
   void DeleteDynamic (Uint32 id);
   cStaticObject * AddStatic ();
   cGroundObject * AddGround ();


   objectlist_t * GetList(void) { return &objectlist; }
   
   int GetCount(void) { return objectlist.size(); }
   
   void Sort();

protected:

};


#endif //_STATICOBJECTS_H_
