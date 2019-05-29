//
// File: GumpLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/GumpLoader.h"
#include "loaders/VerdataLoader.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;

cGumpLoader * pGumpLoader = NULL;

cGumpLoader::cGumpLoader (std::string filename, std::string indexname)
{
    string errstr;

    gumpfile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!gumpfile) {
      pDebug.Log("Out of Memory in GroundTextureLoader::GroundTextureLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    gumpindex = new ifstream(indexname.c_str(), ios::in | ios::binary);
    if(!gumpindex) {
      delete gumpfile;
      gumpfile = NULL;
      pDebug.Log("Out of Memory in GroundTextureLoader::GroundTextureLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    errstr = "Could not load ground texture file: ";
    if (!gumpfile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete gumpfile;
	delete gumpindex;
	gumpfile = NULL;
	gumpindex = NULL;
	return;
    }
    
    if (!gumpindex->is_open()) {
	errstr += indexname;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete gumpfile;
	delete gumpindex;
	gumpfile = NULL;
	gumpindex = NULL;
	return;
    }
      
    gumpindex->seekg(0, ios::end);
    unsigned long idxE = gumpindex->tellg();
    gump_count = idxE / 12;
}

cGumpLoader::~cGumpLoader ()
{
	delete gumpfile;
	delete gumpindex;
}

Texture * cGumpLoader::LoadGump(int index)
{
  if((index < 0) || ((unsigned int)index >= gump_count))
    return NULL;
    
  
  struct sPatchResult patch;
  patch.file = NULL;
   
  if (pVerdataLoader) 
  	patch = pVerdataLoader->FindPatch(VERDATAPATCH_GUMP, index);

  struct stIndexRecord idx;
  
  if (patch.file) {
  	idx = patch.index;
  } else {
        patch.file = gumpfile;
  	gumpindex->seekg(index * 12, ios::beg);
  	gumpindex->read((char *) &idx, sizeof(struct stIndexRecord));
  }

  if(idx.offset == 0xffffffff)
    return NULL;

  Uint16 width = ((idx.extra >> 16) & 0xFFFF);
  Uint16 height = (idx.extra & 0xFFFF);
  
  if(width > 1024 || height > 1024)
    return NULL;

  Uint32 w = width;
  Uint32 h = height;
  
  if (w & 1) w++;

  Texture * texture = new Texture;
  texture->Create16(w, h);
  SDL_Surface * surface = texture->GetSurface();

/*  Uint16 * data = (Uint16 *) malloc (w * h * 2);
  memset(data, 0, w * h * 2); */
  Uint32 * heightTable = new Uint32[height];
  
  patch.file->seekg(idx.offset, ios::beg);
  patch.file->read((char *) heightTable, height * 4);
  
  unsigned int rle = 0;
	
  for (unsigned short y = 0; y < height; y++) {
    gumpfile->seekg(heightTable[y] * 4 + idx.offset, ios::beg);

    unsigned short x = 0;

    // Start of this row (row per row)
    Uint16 *linedata = ((Uint16 *) surface->pixels)  + y * w;

    while (x < width) {
      patch.file->read((char *) &rle, 4);	// Read a RLE key

      unsigned short length = (rle >> 16) & 0xFFFF;	// First two byte
      unsigned short color = rle & 0xFFFF;	// Second two byte

//      Uint32 col32 = color15to32(color);

      for (unsigned int i = 0; i < length; i++) {
	*(linedata++) = color;
	x++;
      }
    }
  }

  delete heightTable;
  
  
  return texture;
}


bool cGumpLoader::CheckGump(int index)
{
  if((index < 0) || ((unsigned int)index >= gump_count))
    return false;
    
  
  struct sPatchResult patch;
  patch.file = NULL;
   
  if (pVerdataLoader) 
  	patch = pVerdataLoader->FindPatch(VERDATAPATCH_GUMP, index);

  struct stIndexRecord idx;
  
  if (patch.file) {
  	idx = patch.index;
  } else {
        patch.file = gumpfile;
  	gumpindex->seekg(index * 12, ios::beg);
  	gumpindex->read((char *) &idx, sizeof(struct stIndexRecord));
  }

  if(idx.offset == 0xffffffff)
    return false;
  return true;
}
