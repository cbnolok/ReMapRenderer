//
// File: Renderer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Renderer.h"
#include "renderer/SDLScreen.h"


#include "include.h"
#include "Config.h"
#include "Debug.h"

#include <math.h>
#include <iostream>

#include "renderer/TextureBuffer.h"
#include "renderer/MapBlock.h"
#include "renderer/MapBuffer.h"

using namespace std;

extern SDLScreen *SDLscreen;

Renderer::Renderer()
{
  flags = 0;
  view_distance = 6;
  do_redraw = false;
  static_buffer = NULL;
  ground_buffer = NULL;
  cache_x = 0;
  cache_y = 0;
}

Renderer::~Renderer()
{
  if (static_buffer) {
    texture_mem -= static_buffer->w *static_buffer->h * 2;
  	SDL_FreeSurface(static_buffer);
  }
  if (ground_buffer) {
    texture_mem -= ground_buffer->w *ground_buffer->h * 2;
  	SDL_FreeSurface(ground_buffer);
  }
}

int Renderer::Init(void)
{
 if (static_buffer) {
    texture_mem -= static_buffer->w *static_buffer->h * 2;
     SDL_FreeSurface(static_buffer);
 }
  if (ground_buffer) {
    texture_mem -= ground_buffer->w *ground_buffer->h * 2;
  	SDL_FreeSurface(ground_buffer);
  }
  cache_x = (nConfig::width + 175) / 176 + 2;
  cache_y = (nConfig::height + 175) / 176 + 2;
  static_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE , cache_x * 176, cache_y * 176, 16, SDL_RGB_16_BITMASK);
  ground_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE , cache_x * 176, cache_y * 176, 16, SDL_RGB_16_BITMASK);
  texture_mem += cache_x * 176 * cache_y * 176 * 2 * 2;
  do_redraw = true;
  if (!static_buffer) {
	pDebug.Log("Out of Memory in Renderer::Init ()", __FILE__, __LINE__, LEVEL_ERROR );
	return false;
  }  	
	return true;
}

int Renderer::DeInit(void)
{
     if (static_buffer) {
        texture_mem -= static_buffer->w *static_buffer->h * 2;
    	SDL_FreeSurface(static_buffer);
    }
     if (ground_buffer) {
        texture_mem -= ground_buffer->w *ground_buffer->h * 2;
    	SDL_FreeSurface(ground_buffer);
    }
	static_buffer = NULL;
	ground_buffer = NULL;
	return true;
}

void Renderer::RenderTerrain(SDL_Surface * target, int mx, int my)
{
  	for (int x = nConfig::minblockx; x <= nConfig::maxblockx; x++) 
  for (int y = nConfig::minblocky; y <= nConfig::maxblocky; y++)
	{
  		int tx = x - nConfig::minblockx;
  		int ty = nConfig::maxblocky - y;
  		
  		int dx = (tx+ty)*176 + hotspotx + mx;
  		int dy = (tx-ty)*176 + hotspoty + my;
  		
  		if (!((dx > 1024 * 2) || (dx < -1024 * 2) || (dy > 3500) || (dy < -3500))) {

  		    cMapblock * block = pMapbuffer->CreateBlock(x, y);
  		
    		if (block) 
     			block->RenderGround(dx, dy, SDLscreen->screen, NULL);
  		}
	} 
}

void Renderer::RenderStatics(SDL_Surface * target, int mx, int my)
{
  	for (int x = nConfig::minblockx; x <= nConfig::maxblockx; x++) 
  for (int y = nConfig::minblocky; y <= nConfig::maxblocky; y++)
	{
  		int tx = x - nConfig::minblockx;
  		int ty = nConfig::maxblocky - y;
  		
  		int dx = (tx+ty)*176 + hotspotx + mx;
  		int dy = (tx-ty)*176 + hotspoty + my;
  		
  		if (!((dx > 1024 * 2) || (dx < -1024 * 2) || (dy > 3500) || (dy < -3500))) {

  		cMapblock * block = pMapbuffer->CreateBlock(x, y);
  		

  		if (block) 
  			block->RenderStatics(dx, dy, SDLscreen->screen, NULL, TILE_CLASS_ITEM);
  		}
	} 
}

void Renderer::Rebuild (int mx, int my)
{
  		RenderTerrain(NULL,mx, my);
//  		SDL_BlitSurface(ground_buffer, NULL, static_buffer, NULL);
  		RenderStatics(NULL, mx, my);
		//SDLscreen->DrawSurface(static_buffer, 0, 0);
}

void Renderer::RecalcHeight()
{
  INT_Rect outbox;
  outbox.x = 0;
  outbox.y = 0;
  outbox.w = 0;
  outbox.h = 0;

  printf("Calculating render size in pixels (non-zoomed)...");
  for (int x = nConfig::minblockx; x <= nConfig::maxblockx; x++) 
  for (int y = nConfig::minblocky; y <= nConfig::maxblocky; y++)
	{
  		cMapblock * block = pMapbuffer->CreateBlock(x, y);
  		int tx = x - nConfig::minblockx;
  		int ty = nConfig::maxblocky - y;
  		
  		if (block) {
  		    block->GetBounds(&outbox, (tx+ty)*176, (tx-ty)*176);
//  			block->RenderStatics((tx+ty)*176, (tx-ty)*176, SDLscreen->screen, NULL, TILE_CLASS_ITEM);
  		}
  	}
   nConfig::width = outbox.w;
   nConfig::height = outbox.h;
   hotspotx = - outbox.x;
   hotspoty = - outbox.y;
   printf(" %i x %i\n", outbox.w, outbox.h);
}
