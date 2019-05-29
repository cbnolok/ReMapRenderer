//
// File: Node.h
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

#ifndef _NODE_H_
#define _NODE_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <map>
#include <vector>
#include <list>

#include "StaticObjects.h"

class cNode
{
  private:
    Uint32 id;
    std::vector <void *> quads;
  public:
        float normal[3];
        float vertex[3];
  	cNode (Uint32 id);
	~cNode ();
	void AddQuad(void * quad);
  protected:

};

typedef struct {
	Uint32 id;
	cNode * nodes[4];
	float normal[3];
	Uint32 texture;
        float texcoords[4][2];
	struct sStaticObject * object;
} sNodeQuad;

typedef std::map<Uint32, cNode *>  nodelist_t;
typedef std::list<sNodeQuad *>  nodequadlist_t;


class cNodeList
{
private:
   nodelist_t nodes[4][4];
public:
   cNodeList ();
   ~cNodeList ();

   void Clear();
   cNode * AddNode(float x, float y, float z);
   
protected:

};

class cNodeQuadList
{
private:
   nodequadlist_t quads;
   bool _transparent;
public:
   cNodeQuadList ();
   ~cNodeQuadList ();

   void Clear();
   sNodeQuad * AddQuad(void);
   
   //void Sort();
      
   bool transparent() { return _transparent; }
   
protected:

};

#endif //_CAMERA_H_
