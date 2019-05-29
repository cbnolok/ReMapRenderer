//
// File: AnimLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/AnimLoader.h"
#include "loaders/VerdataLoader.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;

cAnimLoader * pAnimLoader = NULL;

cAnimLoader::cAnimLoader (std::string filename, std::string indexname)
{
    string errstr;

    animfile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!animfile) {
      pDebug.Log("Out of Memory in AnimLoader::AnimLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    animindex = new ifstream(indexname.c_str(), ios::in | ios::binary);
    if(!animindex) {
      delete animfile;
      animfile = NULL;
      pDebug.Log("Out of Memory in AnimLoader::AnimLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    errstr = "Could not load ground texture file: ";
    if (!animfile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete animfile;
	delete animindex;
	animfile = NULL;
	animindex = NULL;
	return;
    }
    
    if (!animindex->is_open()) {
	errstr += indexname;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete animfile;
	delete animindex;
	animfile = NULL;
	animindex = NULL;
	return;
    }
      
    animindex->seekg(0, ios::end);
    unsigned long idxE = animindex->tellg();
    anim_count = idxE / 12;
}

cAnimLoader::~cAnimLoader ()
{
	delete animfile;
	delete animindex;
}

cAnimation * cAnimLoader::LoadAnimation(int id)
{
  if((id < 0) || ((unsigned int)id >= anim_count))
    return NULL;
    
  struct sPatchResult patch;
  patch.file = NULL;
   
 /* if (pVerdataLoader) 
  	patch = pVerdataLoader->FindPatch(VERDATAPATCH_ANIM, index); */

  struct stIndexRecord idx;
  
  if (patch.file) {
  	idx = patch.index;
  } else {
        patch.file = animfile;
  	animindex->seekg(id * 12, ios::beg);
  	animindex->read((char *) &idx, sizeof(struct stIndexRecord));
  }

  if(idx.offset == 0xffffffff)
    return NULL;
  
  Uint8 * filedata = new Uint8[idx.length];
  Uint8 * filep = filedata;
  Uint8 * eof = filedata + idx.length;

  patch.file->seekg(idx.offset, ios::beg);
  patch.file->read((char *) filedata, idx.length);
  
  
  Uint16 * pal = (Uint16*) filedata; filep += 512;
  Uint32 pal32[256];
  
  int i;
  for (i = 0; i < 256; i++) 
    pal32[i] = htonl(color15to32(pal[i]));
  
  Uint32 framecount = *((Uint32 *) filep); filep += 4;
  Uint32 * lookuptable = (Uint32 *) filep; filep += framecount * 4;
  

  if (!framecount)
    return NULL;
  
  cAnimation * result = new cAnimation;
  for (int index = 0; index < framecount; index ++) {
    filep = filedata + 0x200 + lookuptable[index];
    sAnimationFrame * frame = result->AddFrame();
    frame->centerx = *((Sint16 *) filep); filep += 2;
    frame->centery = *((Sint16 *) filep); filep += 2;
    Uint16 width = *((Uint16 *) filep); filep += 2;
    Uint16 height = *((Uint16 *) filep); filep += 2;
    frame->surface = SDL_CreateRGBSurface(SDL_SWSURFACE , width, height, 32, SDL_BITMASK);
    texture_mem += frame->surface->w * frame->surface->h * 4;
    Uint32 header = *((Uint32 *) filep); filep +=4;
    int j = 0;
    while ((header != 0x7fff7fff) && (filep < eof)) {
      int xrun = header & 0xfff;
      int xoffset = (header >> 22) & 1023;
      int yoffset = (header >> 12) & 1023;

      if(xoffset & 512)
	xoffset = (int) ((unsigned int) xoffset | (0xffffffff - 511));
      if(yoffset & 512)
	yoffset = (int) ((unsigned int) yoffset | (0xffffffff - 511));


	    int x = xoffset + frame->centerx;
        int y = yoffset + frame->centery + height;
        
        Uint32 * data = (Uint32 *) frame->surface->pixels;
        Uint32 * dst = data + y * width + x;
        
        for (i = 0; i < xrun; i++) {
            *dst = pal32[*filep];
            filep++;          
            dst++;
        }
        header = *((Uint32 *) filep); filep +=4;
    }
  }

  delete filedata;
  return result;
}



