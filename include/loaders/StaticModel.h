//
// File: StaticModel.h
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

#ifndef _STATICMODEL_H_
#define _STATICMODEL_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include "GL/gl.h"
#include "GL/glu.h"
#include "../Buffer.h"
#include "StaticTextures.h"
#include "../uotype.h"
#include "../renderer/Node.h"

  struct QuadVertex {
   float vertex[3];
   float u, v;
   float normal[3];
  };

  struct Quad {
   int texture;
   int marked;
   struct QuadVertex vertieces[4];
   float texcoords[2]; /* unused */
   float texsize;
   int gltex;
   int flags;
  };

  struct ModelInfos {
    int height;
    unsigned int flags;
    unsigned char weight;
    unsigned char quality;
    unsigned char quantity;
    unsigned short animid;
    unsigned char hue;
    char name[20];
    char dummy[6];
  };

class StaticModel : public BufferEntry
{
private:
 struct Quad * quads;
 int quadcount;
 int modelindex;
 StaticTextures * textures;
 int groundtex;
 float boundaring_box_min[3];
 float boundaring_box_max[3];
 void CalcNormals (void);
 void AddQuadToNodes (float x, float y, float z, struct Quad * quad, cNodeList * nodes, cNodeQuadList * nodequads, struct sStaticObject * object);
 void CreateBoundaries(void);
public:
 struct ModelInfos infos;
    int modelflags;
    StaticModel (int modelindex);
   ~StaticModel ();
   int Load(std::ifstream * stream);
   void SetTextures (StaticTextures * textures);
   int Paint (void);
   int PaintSelection (void);
   void AddToNodes (float x, float y, float z, cNodeList * nodes, cNodeQuadList * nodequads, struct sStaticObject * object);
   void SetTileData(struct TileDataStaticEntry * tiledata);
   
   bool CheckRay(float vecOrigin[3], float vecDir[3], float deltax, float deltay, float deltaz, float & lambda);

   
 float boundaring_sphere[5];
protected:

};


#endif //_STATICMODEL_H_
