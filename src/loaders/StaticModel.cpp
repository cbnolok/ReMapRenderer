//
// File: StaticModel.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "loaders/StaticModel.h"
#include "loaders/TileDataLoader.h"
#include "renderer/TextureBuffer.h"	
#include "Debug.h"
#include "SDL/SDL_image.h"
#include "include.h"
#include "math.h"
#include "uotype.h"
#include "Geometry.h"

using namespace std;

extern int flag_cullbackfaces;

char FILEID_MODEL[] = "CM#2";

struct ModelFileHeader {
  char id[4];
  int quadcount;
  struct ModelInfos infos;
  unsigned int flags;
  unsigned char Reserved[36];
};

float model_tile_coords[4][3] =
  { {0.0f, 0.0f, 0.01f}, {0.0f, 1.0f, 0.01f}, {1.0f, 1.0f, 0.01f}, {1.0f, 0.0f, 0.01f} };
float model_tile_texcoords[4][3] =
  { {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f} };

float ground_normal[3] = { 0.0, 0.0, 1.0 };




StaticModel::StaticModel(int modelindex)
{
  quads = NULL;
  textures = NULL;
  quadcount = 0;
  groundtex = 0;
  this->modelindex = modelindex;
}

StaticModel::~StaticModel()
{
  if(quads)
    free(quads);
}

void StaticModel::SetTileData(struct TileDataStaticEntry *tiledata)
{
  infos.height = tiledata->height;
  infos.flags = tiledata->flags;
  infos.weight = tiledata->weight;
  infos.quality = tiledata->quality;
  infos.quantity = tiledata->quantity;
  infos.animid = tiledata->animid;
  infos.hue = tiledata->hue;
  memcpy(infos.name, tiledata->name, 20);
}

void StaticModel::CreateBoundaries(void)
{

  int i;

  for (i = 0; i < 3; i++) {
    boundaring_box_min[i] = 10000000.0f;
    boundaring_box_max[i] = -10000000.0f;
  }

  for (int index = 0; index < quadcount; index++) {
    struct Quad *quad = &quads[index];
    int vcount = 4;

    if(quad->flags & QUADFLAG_TRIANGLE)
      vcount = 3;
    for (int vertex = 0; vertex < vcount; vertex++)
      for (i = 0; i < 3; i++) {
	if(quad->vertieces[vertex].vertex[i] < boundaring_box_min[i])
	  boundaring_box_min[i] = quad->vertieces[vertex].vertex[i];
	if(quad->vertieces[vertex].vertex[i] > boundaring_box_max[i])
	  boundaring_box_max[i] = quad->vertieces[vertex].vertex[i];
      }
  }
  for (i = 0; i < 3; i++)
    boundaring_sphere[i] =
      (boundaring_box_max[i] + boundaring_box_min[i]) / 2.0f;

  float dx = boundaring_sphere[0] - boundaring_box_max[0];
  float dy = boundaring_sphere[1] - boundaring_box_max[1];
  float dz = (boundaring_sphere[2] - boundaring_box_max[2]);

  boundaring_sphere[3] = (float) sqrt(dx * dx + dy * dy + dz * dz);
  boundaring_sphere[4] = (float) sqrt(dx * dx + dy * dy + (dz * dz * 0.1f * 0.1f));
}


 void StaticModel::AddQuadToNodes (float x, float y, float z, struct Quad * quad, cNodeList * nodes, cNodeQuadList * nodequads, struct sStaticObject * object)
{
  sNodeQuad * nodequad = nodequads->AddQuad();
  int index;
  for (index = 0; index < 4; index++) {
  	struct QuadVertex * vertex = &quad->vertieces[index];
	cNode * node = nodes->AddNode(x + vertex->vertex[0], y + vertex->vertex[1], z + vertex->vertex[2]);
	nodequad->nodes[index] = node;
  	nodequad->texcoords[index][0] = vertex->u;
  	nodequad->texcoords[index][1] = vertex->v;
	node->AddQuad(nodequad);
  }
  struct QuadVertex * vertex = &quad->vertieces[0];
  for (index = 0; index < 3; index ++)
  	nodequad->normal[index] = vertex->normal[index];
  nodequad->texture = quad->gltex;
  nodequad->object = object;
}


void StaticModel::AddToNodes (float x, float y, float z, cNodeList * nodes, cNodeQuadList * nodequads, struct sStaticObject * object)
{
#ifdef COMPILE_INCOMPLETE_CODE
  for (int index = 0; index < quadcount; index++) {
    struct Quad  * quad = &quads[index];
    if(!quad->gltex) {
      if(quad->texture >= 0) {
	quad->gltex = textures->Get(quad->texture);
      } else {
        if (pTextureBuffer) {
		Texture * texture = pTextureBuffer->GetArtTexture(modelindex + 16384);
		if (texture) 
			quad->gltex = texture->GetGLTex();
	}
      }
    }
    
    if (!(quad->flags & QUADFLAG_TRIANGLE)) {
    	AddQuadToNodes(x, y, z, quad, nodes, nodequads, object);
}
    
  
  }
#endif
}

