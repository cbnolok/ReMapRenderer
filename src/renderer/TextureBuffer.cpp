//
// File: TextureBuffer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/TextureBuffer.h"
#include "loaders/GroundTextures.h"
#include "loaders/ArtLoader.h"
#include "Debug.h"
#include "Config.h"

using namespace std;

TextureBuffer * pTextureBuffer = NULL;

TextureBuffer::TextureBuffer()
{
  groundTiles.setMaxSize(nConfig::cache_ground);
  groundTiles.setCacheSize(0x4000);
  groundTiles.setAutofree(true);

  artTiles.setMaxSize(nConfig::cache_art);
  artTiles.setCacheSize(0x4000);
  artTiles.setAutofree(true);
  
  groundTexmaps.setMaxSize(nConfig::cache_texture);
  groundTexmaps.setCacheSize(0x4000);
  groundTexmaps.setAutofree(true);
}

TextureBuffer::~TextureBuffer()
{
}

Texture *TextureBuffer::GetGroundTexture(int index)
{

  if ((index < 0) || (index >= 0x4000))
  	return NULL;

  Texture * result = groundTiles.findEntry(index);

  if(!result) {
    if (!pGroundTextureLoader)
    	return NULL;
	
    result = NULL;//pGroundTextureLoader->LoadTexture(index);
    
    if (!result) {
    	if (!pArtLoader)
    		return NULL;
	result = pArtLoader->LoadArt(index);
    }
    
    if (!result)
    	result = new Texture;
    groundTiles.addEntry(index, result);
  }

  return result;
}

Texture *TextureBuffer::GetArtTexture(int index)
{
  
  if ((index < 0x4000) && (index >= 0))
  	return GetGroundTexture(index);
  
  if ((index < 0x0) || (index >= 65536))
  	return NULL;
  
  Texture *result = NULL;

  result = artTiles.findEntry(index - 0x4000);

  if(!result) {

    if (!pArtLoader)
    	return NULL;
	
    result = pArtLoader->LoadArt(index);
    if (!result) {
    	result = new Texture;
    }
    artTiles.addEntry(index - 0x4000, result);
  }

  return result;
}

Texture *TextureBuffer::GetGroundTexmap(int index)
{

  if ((index < 0) || (index >= 0x4000))
  	return NULL;

  Texture * result = groundTexmaps.findEntry(index);

  if(!result) {
    if (!pGroundTextureLoader)
    	return NULL;
	
    result = pGroundTextureLoader->LoadTexture(index);
    
    if (!result)
    	result = new Texture;
    groundTexmaps.addEntry(index, result);
  }

  return result;
}
