//
// File: Texture.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "renderer/Texture.h"
#include "Debug.h"
#include "include.h"

using namespace std;

Texture::Texture ()
{
 surface = NULL;
}

Texture::~Texture ()
{
  if (surface) {
    texture_mem -= surface->pitch *surface->h;
  	SDL_FreeSurface(surface);
 	}
}


int Texture::LoadFromData(void * data, int width, int height, int bits_per_pixel)
{
  if (surface)
  	SDL_FreeSurface(surface);
	
  surface = NULL;
	
  if (bits_per_pixel != 32) 
  	return false;
  
  int pitch = width * 4;
  

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
  
//  surface = SDL_CreateRGBSurface&*From(data, width, height, bits_per_pixel, pitch, rmask, gmask, bmask, amask);
  surface = SDL_CreateRGBSurface(0, width, height, bits_per_pixel, SDL_BITMASK);
  texture_mem += surface->w * surface->h * 4;
	
  if (!surface) {
	pDebug.Log("Out of Memory in Texture::LoadFromData ()", __FILE__, __LINE__, LEVEL_ERROR );
	return false;
  }  	
  memcpy (surface->pixels, data, width* height * 4);
  

  return (true);
}

int Texture::Create16(int width, int height)
{
  if (surface)
  	SDL_FreeSurface(surface);
	
  surface = NULL;
	
  
  

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
  
//  surface = SDL_CreateRGBSurface&*From(data, width, height, bits_per_pixel, pitch, rmask, gmask, bmask, amask);
  surface = SDL_CreateRGBSurface(SDL_SRCCOLORKEY, width, height, 16, SDL_RGB_16_BITMASK);
  SDL_SetColorKey(surface, SDL_SRCCOLORKEY, 0);
	
  if (!surface) {
	pDebug.Log("Out of Memory in Texture::LoadFromData ()", __FILE__, __LINE__, LEVEL_ERROR );
	return false;
  }  	

  texture_mem += surface->pitch * surface->h;
  return (true);
}

int Texture::LoadFromData16(void * data, int width, int height, int bits_per_pixel)
{
  if (surface)
  	SDL_FreeSurface(surface);
	
  surface = NULL;
	
  
  

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
  
//  surface = SDL_CreateRGBSurface&*From(data, width, height, bits_per_pixel, pitch, rmask, gmask, bmask, amask);
  surface = SDL_CreateRGBSurface(SDL_SRCCOLORKEY, width, height, 16, SDL_RGB_16_BITMASK);
  SDL_SetColorKey(surface, SDL_SRCCOLORKEY, 0);
  texture_mem += surface->w * surface->h * 2;
	
  if (!surface) {
	pDebug.Log("Out of Memory in Texture::LoadFromData ()", __FILE__, __LINE__, LEVEL_ERROR );
	return false;
  }  	
  memcpy (surface->pixels, data, width* height * 2);
  

  return (true);
}

int Texture::LoadFromFile( const char * filename )
{
  	if (surface)
  		SDL_FreeSurface(surface);
	
	surface = NULL;//IMG_Load( filename );

	if( !surface )
	{
		char errorStr[512];
		sprintf( errorStr, "Error loading texture: %s", filename );
		pDebug.Log( errorStr, __FILE__, __LINE__, LEVEL_ERROR );
		return false;
	}


	return true;
}



int Texture::GetHeight(void)
{
  if (!surface)
  	return 0;
  return surface->h;
}


int Texture::GetWidth(void)
{
  if (!surface)
  	return 0;
  return surface->w;
}

