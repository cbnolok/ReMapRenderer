//
// File: MapBuffer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/MapBuffer.h"
#include "Config.h"
#include "Debug.h"
#include "include.h"
#include "renderer/Camera.h"

using namespace std;

cMapbuffer * pMapbuffer = NULL;

cMapbuffer::cMapbuffer()
{
}

cMapbuffer::~cMapbuffer()
{
  Clear();
}

void cMapbuffer::Clear(void)
{
  MapBuffer_t::iterator iter;

  for (iter = root.begin(); iter != root.end(); iter++)
    delete(*iter).second;

  root.clear();
  cache.clear();
}

cMapblock *cMapbuffer::Get(int x, int y)
{
  if((x < 0) || (y < 0))
    return NULL;

  MapBuffer_t::iterator iter;

  iter = root.find((Uint32) x << 16 | y);

  if(iter == root.end())
    return NULL;
  else
    return (*iter).second;
}

void cMapbuffer::Add(cMapblock * block)
{
  if(!block) {
    pDebug.Log("NULL block in Mapbuffer::Add(Mapblock *)", __FILE__, __LINE__,
	       LEVEL_CRITICAL);
    return;
  }

  if ((nConfig::cache_block > 0) && (cache.size() >= nConfig::cache_block)) {
    cMapblock * nblock = cache[0];
    root.erase(nblock->blockx << 16 | nblock->blocky);
    cache.erase(cache.begin());
    delete nblock;
  
  }  


  Uint16 x = block->blockx;
  Uint16 y = block->blocky;

  root.insert(make_pair((Uint32) x << 16 | y, block));
  cache.push_back(block);
}

cMapblock *cMapbuffer::CreateBlock(int x, int y)
{
  cMapblock *block;
  
 
  if ((x < 0) || (y < 0))
  	return NULL; 

  if((block = Get(x, y)))	/* check buffer for block */
    return block;


  /* if it is not there, generate new */

  block = new cMapblock();

  block->Generate(x, y);

  Add(block);

  return block;
}


void cMapbuffer::FreeBuffer(int blockx, int blocky, int radius)
{
  MapBuffer_t::iterator iter;
  

  for (iter = root.begin(); iter != root.end(); iter++)
  {
    if ((iter->second->blockx < blockx - radius) ||
        (iter->second->blockx > blockx + radius) ||
	(iter->second->blocky < blocky - radius) ||
        (iter->second->blocky > blocky + radius)) {
    		delete iter->second;
		root.erase(iter);
		return;
    }
  }

}
