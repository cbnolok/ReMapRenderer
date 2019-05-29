//
// File: GroundTextures.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/GroundTextures.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;



cGroundTextureLoader * pGroundTextureLoader = NULL;

cGroundTextureLoader::cGroundTextureLoader (std::string filename, std::string indexname)
{
    string errstr;

    texmapsfile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!texmapsfile) {
      pDebug.Log("Out of Memory in GroundTextureLoader::GroundTextureLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    texmapsindex = new ifstream(indexname.c_str(), ios::in | ios::binary);
    if(!texmapsindex) {
      delete texmapsfile;
      texmapsfile = NULL;
      pDebug.Log("Out of Memory in GroundTextureLoader::GroundTextureLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    errstr = "Could not load ground texture file: ";
    if (!texmapsfile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete texmapsfile;
	delete texmapsindex;
	texmapsfile = NULL;
	texmapsindex = NULL;
	return;
    }
    
    if (!texmapsindex->is_open()) {
	errstr += indexname;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete texmapsfile;
	delete texmapsindex;
	texmapsfile = NULL;
	texmapsindex = NULL;
	return;
    }
      
    texmapsindex->seekg(0, ios::end);
    unsigned long idxE = texmapsindex->tellg();
    groundtex_count = idxE / 12;
}

cGroundTextureLoader::~cGroundTextureLoader ()
{
	delete texmapsfile;
	delete texmapsindex;
}

Texture * cGroundTextureLoader::LoadTexture(int index)
{
  if((index < 0) || ((unsigned int)index >= groundtex_count))
    return NULL;

  struct stIndexRecord idx;

  texmapsindex->seekg(index * 12, ios::beg);
  texmapsindex->read((char *) &idx, sizeof(struct stIndexRecord));

  if(idx.offset == 0xffffffff)
    return NULL;

  int size = (idx.extra == 1 ? 128 : 64);

  if(idx.length != (unsigned int) (size * size * 2)) {
    return NULL;
  }

  char *sprite = (char *) malloc(size * size * 4);
  Uint16 *temp = (Uint16 *) malloc(size * size * 2);

  texmapsfile->seekg(idx.offset, ios::beg);
  texmapsfile->read((char *) temp, size * size * 2);

  Uint16 *src = temp;
  Uint32 *dst = (Uint32 *) sprite;

  for (int i = 0; i < (size * size); i++) {
    *dst =  color15to32(*src) | 0xff000000;
    dst ++;
    src++;
  }

  free(temp);
  
  Texture * texture = new Texture;
  if (texture) {
  	texture->LoadFromData(sprite, size, size, 32);
  	free(sprite);
  }
  
  return texture;
}
