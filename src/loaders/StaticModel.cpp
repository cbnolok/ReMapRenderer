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

extern bool CheckHeaderID(char *ID1, char *ID2);
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

void StaticModel::SetTextures(StaticTextures * textures)
{
  this->textures = textures;
}

int StaticModel::Load(ifstream * stream)
{
  struct ModelFileHeader header;
  struct Quad quad;

  stream->read((char *) &header, sizeof header);
  if(!CheckHeaderID(header.id, FILEID_MODEL)) {
    pDebug.Log("Unknown Header ID in StaticModel::Load(ifstream *)", __FILE__,
	       __LINE__, LEVEL_WARNING);
    return (false);
  }

  if(quads)
    free(quads);
  quads = NULL;

  quadcount = header.quadcount;
  infos = header.infos;
  modelflags = header.flags;

  if(quadcount) {
    quads = (struct Quad *) malloc((sizeof(struct Quad)) * quadcount);
    if(!quads) {
      pDebug.Log("Out of Memory in StaticModel::Load(ifstream *)", __FILE__,
		 __LINE__, LEVEL_ERROR);
      return (false);
    }
    stream->read((char *) quads, (sizeof(struct Quad)) * quadcount);

    int i, j;

    for (int index = 0; index < quadcount; index++) {
      quads[index].gltex = 0;
      quad = quads[index];
      for (j = 0; j < 4; j++) {
	quads[index].vertieces[j].vertex[0] = quad.vertieces[j].vertex[2];
	quads[index].vertieces[j].vertex[1] =
	  1.0f - quad.vertieces[j].vertex[0];
	quads[index].vertieces[j].vertex[2] =
	  quad.vertieces[j].vertex[1] * 10.0f;
	quads[index].vertieces[j].normal[0] = quad.vertieces[j].normal[2];
	quads[index].vertieces[j].normal[1] = quad.vertieces[j].normal[0];
	quads[index].vertieces[j].normal[2] =
	  quad.vertieces[j].normal[1] * 10.0f;
	quads[index].vertieces[j].u = quad.vertieces[j].u;
	quads[index].vertieces[j].v = 1.0 - quad.vertieces[j].v;
      }
      bool can_be_merged = true;
      float qvertex[4][3];

      quad = quads[index];
      if(!(quad.flags & QUADFLAG_TRIANGLE)) {
	for (i = 0; i < 4; i++) {
	  for (j = 0; j < 3; j++)
	    qvertex[i][j] =
	      quad.vertieces[i].vertex[j] -
	      quad.vertieces[(i + 1) & 3].vertex[j];
	}
	for (i = 0; i < 3; i++)
	  can_be_merged &=
	    (((qvertex[i][0] * qvertex[i + 1][0]) +
	      (qvertex[i][1] * qvertex[i + 1][1]) +
	      (qvertex[i][2] * qvertex[i + 1][2])) == 0);

	if(can_be_merged)
	  quads[index].flags |= QUADFLAG_CAN_BE_MERGED;
      }
    }

  }

  if(modelflags & MODELFLAG_TILEABLE)
    CalcNormals();

  if (pTileDataLoader) {
  	struct TileDataStaticEntry entry;
	if (pTileDataLoader->LoadEntry(modelindex, &entry)) 
		SetTileData(&entry);
  }

  CreateBoundaries();

  return (true);
}

int StaticModel::Paint(void)
{

  for (int index = 0; index < quadcount; index++) {
    if(!quads[index].gltex) {
      if(quads[index].texture >= 0) {
	quads[index].gltex = textures->Get(quads[index].texture);
      } else {
        if (pTextureBuffer) {
		Texture * texture = pTextureBuffer->GetArtTexture(modelindex + 16384);
		if (texture) 
			quads[index].gltex = texture->GetGLTex();
	}
      }
    }

    
    
    glBindTexture(GL_TEXTURE_2D, quads[index].gltex);

    if((quads[index].flags & QUADFLAG_BACKFACE) & (flag_cullbackfaces)) {
      glDisable(GL_CULL_FACE);
      flag_cullbackfaces = false;
    } else
      if((!(quads[index].flags & QUADFLAG_BACKFACE)) & (!flag_cullbackfaces))
    {
      glEnable(GL_CULL_FACE);
      flag_cullbackfaces = true;
    }

    int vmax = 3;

    if(quads[index].flags & QUADFLAG_TRIANGLE) {
      vmax = 2;
      glBegin(GL_TRIANGLES);
    } else {
      glBegin(GL_QUADS);
    }

    for (int vertex = 0; vertex <= vmax; vertex++) {
      struct QuadVertex *qd = &quads[index].vertieces[vertex];

      glNormal3fv(qd->normal);
      glTexCoord2f(quads[index].vertieces[vertex].u,
		   quads[index].vertieces[vertex].v);
      glVertex3fv(qd->vertex);
    }

    glEnd();


  }

  if(modelflags & MODELFLAG_ISGROUND) {
    if(flag_cullbackfaces) {
      glDisable(GL_CULL_FACE);
      flag_cullbackfaces = false;
    }

    if ((!groundtex) && pTextureBuffer) {
       Texture * texture = pTextureBuffer->GetArtTexture(modelindex + 16384);
      groundtex =
	texture->GetGLTex();
    }
    glBindTexture(GL_TEXTURE_2D, groundtex);

    glBegin(GL_QUADS);
    for (int vertex = 0; vertex <= 3; vertex++) {
      glNormal3fv(ground_normal);
      glTexCoord2fv(model_tile_texcoords[vertex]);
      glVertex3fv(model_tile_coords[vertex]);
    }
    glEnd();

  }

  return true;
}


void StaticModel::CalcNormals(void)
{
  float normal[3];

  for (int index = 0; index < quadcount; index++) {
    CalcNormal(quads[index].vertieces[0].vertex,
	       quads[index].vertieces[1].vertex,
	       quads[index].vertieces[2].vertex, normal);
    for (int vertex = 0; vertex < 4; vertex++)
      for (int k = 0; k < 3; k++)
	quads[index].vertieces[vertex].normal[k] = normal[k];
  }
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
}


int StaticModel::PaintSelection(void)
{

  for (int index = 0; index < quadcount; index++) {
    if((quads[index].flags & QUADFLAG_BACKFACE) & (flag_cullbackfaces)) {
      glDisable(GL_CULL_FACE);
      flag_cullbackfaces = false;
    } else
      if((!(quads[index].flags & QUADFLAG_BACKFACE)) & (!flag_cullbackfaces))
    {
      glEnable(GL_CULL_FACE);
      flag_cullbackfaces = true;
    }

    int vmax = 3;

    if(quads[index].flags & QUADFLAG_TRIANGLE) {
      vmax = 2;
      glBegin(GL_TRIANGLES);
    } else {
      glBegin(GL_QUADS);
    }

    for (int vertex = 0; vertex <= vmax; vertex++) {
      struct QuadVertex *qd = &quads[index].vertieces[vertex];
      glVertex3fv(qd->vertex);
    }

    glEnd();

  }

  if(modelflags & MODELFLAG_ISGROUND) {
    if(flag_cullbackfaces) {
      glDisable(GL_CULL_FACE);
      flag_cullbackfaces = false;
    }

    glBegin(GL_QUADS);
    for (int vertex = 0; vertex <= 3; vertex++) 
      glVertex3fv(model_tile_coords[vertex]);
    
    glEnd();

  }

  return true;
}

bool StaticModel::CheckRay(float vecOrigin[3], float vecDir[3], float deltax, float deltay, float deltaz, float & lambda)
{
  
  float vecs[3][3];
  float act_lambda;
  bool result = false;
  lambda = 100000000.0f;
  for (int index = 0; index < quadcount; index++) {
      for (int j = 0; j < 3; j++) {
        struct QuadVertex *qd = &quads[index].vertieces[j];
	vecs[j][0] = qd->vertex[0] + deltax;
	vecs[j][1] = qd->vertex[1] + deltay;
	vecs[j][2] = (qd->vertex[2] + deltaz) * 0.1f;
      }
      if (IntersectTriangleWithRay(vecOrigin, vecDir, vecs[0], vecs[1], vecs[2], act_lambda)) {
      	result = true;
	if (act_lambda < lambda)
		lambda = act_lambda;
      }
/*      if(!quads[index].flags & QUADFLAG_TRIANGLE) 
      if (IntersectTriangleWithRay(vecOrigin, vecDir, vecs[0], vecs[2], vecs[3], act_lambda)) {
      	result = true;
	if (act_lambda < lambda)
		lambda = act_lambda;
      }*/

    }

  return result;

}
