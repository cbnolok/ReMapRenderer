//
// File: Renderer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Renderer.h"
#include "renderer/SDLScreen.h"


#include "include.h"
#include "Config.h"
#include "Debug.h"

#include "Geometry.h"

#include <math.h>
#include <iostream>

#include "renderer/TextureBuffer.h"
#include "renderer/AnimBuffer.h"
#include "renderer/MapBlock.h"
#include "renderer/MapBuffer.h"

using namespace std;

extern SDLScreen *SDLscreen;

Renderer::Renderer()
{
  flags = 0;
  view_distance = 6;
  m_lightlevel = 1.0f;
  do_redraw = false;
  static_buffer = NULL;
  ground_buffer = NULL;
  cache_x = 0;
  cache_y = 0;
  bufferpos_x = 0;
  bufferpos_y = 0;
  baseblock_x = 0;
  baseblock_y = 0;
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
  bufferpos_x = 176;
  bufferpos_y = 176;
  baseblock_x = nConfig::startx / 8;
  baseblock_y = nConfig::starty / 8;
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

void Renderer::SetRenderFlag(int flag, int value)
{
  switch (flag) {

  case 0:
    if(value)
      flags |= flag;		/* set flag */
    else
      flags &= !flag;		/* reset flag */
    break;

  default:
    pDebug.Log("Unknown Flag in Game::SetRenderFlag(int, int)", __FILE__,
	       __LINE__, LEVEL_ERROR);
  }
}

int Renderer::GetRenderFlag(int flag)
{
  return (flags & flag);
}

void Renderer::Scroll (int x, int y)
{
	bool do_rebuild = false;
	
	bufferpos_x += x;
	if (bufferpos_x >= 176*2) {
		bufferpos_x -= 176; do_rebuild = true; 
		baseblock_x ++;
		baseblock_y --;
		bufferpos_x -= 176;
		RebuildPartX(-2);
	}
	
	if (bufferpos_x < 0) {
		bufferpos_x += 176; do_rebuild = true; baseblock_x --;
		baseblock_y ++;
		bufferpos_x += 176;
		RebuildPartX(2);
	}
	
	bufferpos_y += y;
	
	if (bufferpos_y >= 176*2) {
		bufferpos_y -= 176; do_rebuild = true;
		baseblock_y ++;
		baseblock_x ++;
		bufferpos_y -= 176;
		RebuildPartY(-2);
	}

	
	if (bufferpos_y < 0) {
		bufferpos_y += 176; do_rebuild = true; baseblock_y --;
		baseblock_x --;
		bufferpos_y += 176;
		RebuildPartY(2);
	}
	
	
		//Rebuild ();
		SDL_Rect rect;
		rect.x = bufferpos_x;
		rect.y = bufferpos_y;
		rect.w = nConfig::width;
		rect.h = nConfig::height;
		SDL_BlitSurface(static_buffer, &rect, SDLscreen->screen, NULL);
		SDLscreen->Flip();
		return;
	
}

int Renderer::RenderScene(void)
{	
	
/*	if (pTextureBuffer) 
		texture = pTextureBuffer->GetGroundTexture(0x020); */
//	SDLscreen->DrawSurface(texture->GetSurface(), 10, 10);
    SDLscreen->CheckUpdate();
	if (do_redraw) {
		do_redraw = false;
		SDL_Rect rect;
		rect.x = bufferpos_x;
		rect.y = bufferpos_y;
		rect.w = nConfig::width;
		rect.h = nConfig::height;
		SDL_BlitSurface(static_buffer, &rect, SDLscreen->screen, NULL);
		SDLscreen->Flip();
		return true;
	}
	
	if (SDLscreen->do_redraw) {
	printf("%i\n", texture_mem);
//		Rebuild();
		SDL_Rect rect;
		rect.x = bufferpos_x;
		rect.y = bufferpos_y;
		rect.w = nConfig::width;
		rect.h = nConfig::height;
		SDL_BlitSurface(static_buffer, &rect, SDLscreen->screen, NULL);
		SDLscreen->do_redraw = false;
		SDLscreen->Flip();
		return true;
	}
	
	
/*  if (!pMapbuffer || !pCamera)
  	return false;
  
  binds = 0;
  
  HandleStaticFaders();
	
  glEnable(GL_CULL_FACE);
  flag_cullbackfaces = true;
  
  
  SDLscreen->ClearScreen();
  SDLscreen->SetLight(m_lightlevel);

                     
  if (pClient) {
	cCharacter * player_character = pClient->player_character();
	if (player_character) {
		float cam_x, cam_y, cam_z;
		float player_x, player_y, player_z;
		pCamera->GetGamePosition(cam_x, cam_y, cam_z);
		player_character->getPosition(player_x, player_y, player_z);
		if ((player_x != cam_x) || (player_y != cam_y) || (player_z != cam_z)) {
    		pCamera->SetX(-(player_x - (int) (player_x / 8.0f) * 8));
    		pCamera->SetY(-(player_y - (int) (player_y / 8.0f) * 8));
    		pCamera->SetZ(-player_z);

    		pCamera->SetBlockX((int) player_x / 8);
    		pCamera->SetBlockY((int) player_y / 8);
		}

	}
  }

  bool do_culling = pCamera->CamHasChanged() != 0;
  
  pCamera->PlaceGLMatrix();
  
  if (do_culling)
  	ExtractFrustum();
	
  static int old_z = ROOF_NONE;
  if (do_culling || (old_z == ROOF_WAIT)) {
  	int z = GetRoofHeight();
	
	if (z != old_z) {
		if (static_faders.size ()) {
			old_z = ROOF_WAIT;
		} else {
		cFader * fader = new cFader (255.0f, 50.0f, 1000);
		fader->Start();
		int count = SetFader(z, 255, fader, false);
		if (count)
			static_faders.push_back(fader);
		else
			delete fader;
			
		fader = new cFader (50.0f, 255.0f, 1000);
		fader->Start();
		count = SetFader(z, 50, fader, true);
		if (count)
			static_faders.push_back(fader);
		else
			delete fader;
		old_z = z;
		}
	}
  }

  glPushMatrix();
  RenderTerrain(do_culling);
  RenderDynamics(do_culling);
  RenderCharacters(do_culling);
  if (drag_model)
  	RenderDragModel();
  RenderWater(do_culling);
  RenderTransparent();
  glPopMatrix();

  if (pUOGUI)
  	pUOGUI->Draw();
	
	  
  return SDLscreen->DrawGL(); */
}

void Renderer::RebuildPartX(int dir) {
}

void Renderer::RebuildPartY(int dir) {
}

void Renderer::RenderTerrain(SDL_Surface * target, int mx, int my)
{

  int blockx = baseblock_x;
  int blocky = baseblock_y;
  
  int height = nConfig::maxblocky - nConfig::minblocky;

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

  
  int height = nConfig::maxblocky - nConfig::minblocky;

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




void Renderer::RenderDynamics(bool do_culling)
{
/*  if (!pCamera || !pDynamicObjectList || !pStaticEngine)
  	return;

  int blockx = pCamera->GetBlockX();
  int blocky = pCamera->GetBlockY();

  int min_x = (blockx - view_distance) * 8;
  int min_y = (blocky - view_distance) * 8;
  int max_x = (blockx + view_distance) * 8;
  int max_y = (blocky + view_distance) * 8;
  int dx = blockx * 8;
  int dy = blocky * 8;

  dynamiclist_t * dynamics = pDynamicObjectList->GetList();
  dynamiclist_t::iterator iter;

  for (iter = dynamics->begin(); iter != dynamics->end(); iter++) {
		cDynamicObject * object = iter->second;
		if ((object->x >= min_x) && (object->y >= min_y) && (object->x <= max_x) 
						&& (object->y <= max_y)) 
			{
				StaticModel * model = pStaticEngine->Get(object->model);
				if (model) {
					glPushMatrix();
					glTranslatef(object->x - dx, object->y - dy, object->z);
  					model->Paint();
					glPopMatrix();
				}
			}
  } */
}


void Renderer::RenderCharacters(bool do_culling)
{
  /*if (!pCamera || !pCharacterList || !pStaticEngine)
  	return;

  int blockx = pCamera->GetBlockX();
  int blocky = pCamera->GetBlockY();

  int min_x = (blockx - view_distance) * 8;
  int min_y = (blocky - view_distance) * 8;
  int max_x = (blockx + view_distance) * 8;
  int max_y = (blocky + view_distance) * 8;
  int dx = blockx * 8;
  int dy = blocky * 8;

  characterlist_t * characters = pCharacterList->GetList();
  characterlist_t::iterator iter;
  Uint32 currentticks = SDL_GetTicks();

  for (iter = characters->begin(); iter != characters->end(); iter++) {
		cCharacter * character = iter->second;
		if ((character->x() >= min_x) && (character->y() >= min_y) && (character->x() <= max_x) 
						&& (character->y() <= max_y)) 
			{
					glPushMatrix();
					glTranslatef(character->fx() - dx, character->fy() - dy, character->fz());
					glScalef(-1.0f, 1.0f, 10.0f);
					glTranslatef(-0.5f, 0.5f, 0.0f);
					glRotatef(- character->angle(), 0.0f, 0.0f, 1.0f);
					float curtime = character->animtime();
					float delta_time = (currentticks - character->lastanim()) *  0.001f;
					if (delta_time > 0.1f) delta_time = 0.1f;
					character->setLastAnim(currentticks);
					curtime += delta_time;
					if (pGrannyLoader) {
						SDLscreen->SetHue(character->hue());
						pGrannyLoader->Render(400, 3, curtime);

						for (int layer = 0; layer < 25; layer++) {
							cCharacterEquip * equip = character->GetEquip(layer);
							if (equip) {
								SDLscreen->SetHue(equip->hue());
								pGrannyLoader->Render(equip->anim(), 3, curtime);
							}
						}

						SDLscreen->SetHue();
						
					}
					character->RenderText();
					character->setAnimtime(curtime);
					glPopMatrix();
			}
  }


*/
}


int Renderer::GetRoofHeight()
{
/*	if (!pCamera)
		return ROOF_NONE;
		
	Uint32 x, y;
	int z;
	pCamera->GetGamePosition(x, y, z);
	
	cMapblock * block = pMapbuffer->CreateBlock(x / 8, y / 8);
  	if (block) {
		if (block->GetRoofHeight(x % 8, y % 8, z) != ROOF_NONE)
			return z + 15;
	}
	
	return ROOF_NONE; */
}


void Renderer::TeleportTo(int x, int y)
{
      baseblock_x = x / 8;
      baseblock_y = y / 8;
      do_redraw = true;
}

void Renderer::RecalcHeight()
{
  int height = nConfig::maxblocky - nConfig::minblocky;
  INT_Rect outbox;
  outbox.x = 0;
  outbox.y = 0;
  outbox.w = 0;
  outbox.h = 0;

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
   printf("Bitmap Size: %i %i\n", outbox.w, outbox.h);
}

