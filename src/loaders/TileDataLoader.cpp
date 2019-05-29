//
// File: TileDataLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/TileDataLoader.h"
#include "Debug.h"
#include <string.h>
#include <iostream>

using namespace std;

cTileDataLoader * pTileDataLoader = NULL;

cTileDataLoader::cTileDataLoader (std::string filename)
{
    string errstr;

    tiledatafile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!tiledatafile) {
      pDebug.Log("Out of Memory in ArtLoader::ArtLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
  
    errstr = "Could not load ground tiledata file: ";
    if (!tiledatafile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete tiledatafile;
	tiledatafile = NULL;
	return;
    }
 
}

cTileDataLoader::~cTileDataLoader ()
{
	delete tiledatafile;
}

bool cTileDataLoader::LoadEntry (Uint32 index, struct TileDataStaticEntry * entry)
{
  if ((index >= 49152) || !entry || !tiledatafile)
    return false;

  int group = index / 32;
  int groupidx = index % 32;

  tiledatafile->seekg(512 * 836 + 1188 * group + 4 + groupidx * 37,
			    ios::beg);
  tiledatafile->read((char *) entry, 37);

  return true;

}
