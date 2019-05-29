//
// File: UOMap.cpp
// Created by:  Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "loaders/UOMap.h"
#include <fstream>
#include <cstring>
#include "Debug.h"
#include "Config.h"
#include <stdlib.h> // doc: malloc and free
#include "loaders/LocalMapPatches.h"

using namespace std;

MapLoader * pMapLoader = NULL;


UOMapLoader::UOMapLoader(char *mapfile, char *staticfile, char *staidx,
			 int type)
{
  mapstream = new ifstream(mapfile, ios::in | ios::binary);
  if(!mapstream) {
    pDebug.
      Log
      ("Out of Memory in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  if(!mapstream->is_open()) {
    printf("While loading %s: \n", mapfile);
    pDebug.
      Log
      ("Map load error in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  staticstream = new ifstream(staticfile, ios::in | ios::binary);
  if(!staticstream) {
    pDebug.
      Log
      ("Out of Memory in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  if(!staticstream->is_open()) {
    printf("While loading %s: \n", staticfile);
    pDebug.
      Log
      ("Static load error in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  staidxstream = new ifstream(staidx, ios::in | ios::binary);
  if(!staidxstream) {
    pDebug.
      Log
      ("Out of Memory in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  if(!staidxstream->is_open()) {
    printf("While loading %s: \n", staidx);
    pDebug.
      Log
      ("Static index load error in UOMapLoader::UOMapLoader(char *, char * , char *, int)",
       __FILE__, __LINE__, LEVEL_ERROR);
    return;
  }

  if(type == UOMAP_MAP2) {
    width = 288;
    height = 200;
  } else {
    width = 768;
    height = 512;
  }

  patches = new LocalMapPatches;
//  patches = NULL;
}

UOMapLoader::~UOMapLoader()
{
  mapstream->close();
  delete mapstream;

  staticstream->close();
  delete staticstream;

  staidxstream->close();
  delete staidxstream;

  if (patches)
  	delete patches;


}

void UOMapLoader::LoadMapBlock(int x, int y, MulBlock * block)
{
  if((x >= width) || (y >= height) || (x < 0) || (y < 0)) {
    memset((char *) block, 0, sizeof(struct MulBlock));
    return;
  }

  mapstream->seekg(((x * height) + y) * 196 + 4, ios::beg);
  mapstream->read((char *) block, 192);
}

/*void UOMapLoader::LoadStaticIdx(int x, int y, stIndexRecord * block)
{
  if((x >= width) || (y >= height) || (x < 0) || (y < 0)) {
    memset((char *) block, 0, sizeof(stIndexRecord));
    return;
  }

  mutex_idx.MutexLock();

  staidxstream->seekg((x * height + y) * 12, ios::beg);
  staidxstream->read((char *) block, 12);

  mutex_idx.MutexUnlock();
}

void UOMapLoader::LoadStatics(int ofs, int len)
{
  mutex_statics.MutexLock();

  staticstream->seekg(ofs, ios::beg);
  staticstream->read((char *) statics, len);

  mutex_statics.MutexUnlock();
}*/

struct staticinfo * UOMapLoader::LoadStatics(int x, int y, int &len)
{
  stIndexRecord block;
  if((x >= width) || (y >= height) || (x < 0) || (y < 0))
    return NULL;


  staidxstream->seekg((x * height + y) * 12, ios::beg);
  staidxstream->read((char *) &block, 12);

  if (block.length == 0xffffffff)
  	return NULL;

  len = block.length / 7;

  struct staticentry * uostatics = (struct staticentry *) malloc(block.length);
  struct staticinfo * static_infos = (struct staticinfo *) malloc(len * sizeof (struct staticinfo));


  staticstream->seekg(block.offset, ios::beg);
  staticstream->read((char *) uostatics, block.length);


  struct staticentry * uo_p = uostatics;
  struct staticinfo * info_p =  static_infos;

  for (int index=0; index < len; index++) {
  	info_p->x = uo_p->x;
  	info_p->y = uo_p->y;
  	info_p->z = uo_p->z;
  	info_p->TileID = uo_p->TileID;
	info_p->Hue = uo_p->Hue;
	info_p->obj_id = index;
	info_p ++;
	uo_p ++;
  }


  free(uostatics);
  if (patches) {
  	int oldlen = len;
	bool apply_needed = false;
  	len = patches->GetNewStaticSize(len, x, y, apply_needed);

	if (apply_needed) {
		struct staticinfo * statics_unpatched = static_infos;

		static_infos = (struct staticinfo *) malloc(len * sizeof (struct staticinfo));
		memset(static_infos, 0, len * sizeof (struct staticinfo));

		patches->ApplyPatch(x, y, oldlen, len, statics_unpatched, static_infos);

		free(statics_unpatched);
	}

  }

  return static_infos;
}


unsigned int UOMapLoader::AddObject(unsigned int x, unsigned int y, int z, unsigned int tileid)
{
	if (!patches)
		return 0;


	struct StaticPatch patch;
	memset(&patch, 0, sizeof (patch));
	patch.type = PATCH_ADD;
	patch.x = x % 8;
	patch.y = y % 8;
	patch.blockx = x / 8;
	patch.blocky = y / 8;
	patch.z = z;
	patch.tileid = tileid;

	patch.obj_id = patches->GetNewID(patch.blockx, patch.blocky);

	patches->AddPatch(patch);

	return patch.obj_id;


}

unsigned int UOMapLoader::DelObject(unsigned int blockx, unsigned int blocky, unsigned int obj_id)
{
	if (!patches)
		return 0;


  stIndexRecord block;
  if((blockx >= (unsigned int)width) || (blocky >= (unsigned int)height))
    return 0;


  staidxstream->seekg((blockx * height + blocky) * 12, ios::beg);
  staidxstream->read((char *) &block, 12);


  int len = block.length / 7;

  if (block.length == 0xffffffff)
  	len = 0;

  if (obj_id < len) {
	struct StaticPatch patch;
	memset(&patch, 0, sizeof (patch));
	patch.type = PATCH_DELETE;
	patch.obj_id = obj_id;
	patch.blockx = blockx;
	patch.blocky = blocky;

	patches->AddPatch(patch);

  } else {
  	patches->DelPatch(blockx, blocky, obj_id);
  }

  return 0;


}


void UOMapLoader::Save(char * filename)
{
	if (patches)
		((LocalMapPatches*)patches)->Save(filename);
}

void UOMapLoader::Load(char * filename)
{
	if (patches)
		((LocalMapPatches*)patches)->Load(filename);
}

