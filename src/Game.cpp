//
// File: Game.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include <stdio.h>
#include <math.h>

#include "Game.h"
#include "Debug.h"
#include "Config.h"

#include "renderer/SDLScreen.h"

#include "loaders/UOMap.h"
#include "loaders/GroundTextures.h"
#include "loaders/GumpLoader.h"
#include "loaders/AnimLoader.h"
#include "loaders/VerdataLoader.h"
#include "loaders/ArtLoader.h"
#include "loaders/FontLoader.h"
#include "loaders/HueLoader.h"
#include "loaders/SkillLoader.h"
#include "loaders/TileDataLoader.h"

#include "renderer/MapBuffer.h"
#include "renderer/TextureBuffer.h"
#include "renderer/AnimBuffer.h"
#include "renderer/DynamicObjects.h"
#include "renderer/Characters.h"

using namespace std;

Game pGame;

extern SDLScreen *SDLscreen;



Game::Game()
{
  renderer = NULL;
  m_paused = false;
}

Game::~Game()
{

  if(renderer)
    delete renderer; 

}

void Game::LoadMuls(std::string mulpath)
{
	if (mulpath.size()) {
		char last_char = mulpath[mulpath.length() - 1];
		if ((last_char != '/') && (last_char != '\\'))
			mulpath += "/";
	}
	
	string mul_map0 = mulpath+"map0.mul";
	string mul_statics0 = mulpath+"statics0.mul";
	string mul_staidx0 = mulpath+"staidx0.mul";
	string mul_texmaps = mulpath+"texmaps.mul";
	string mul_texidx = mulpath+"texidx.mul";
       
        pDebug.Log("SYS | Loading UO Files....");	
        pMapLoader = new UOMapLoader((char*)mul_map0.c_str(), (char*)mul_statics0.c_str(), (char*)mul_staidx0.c_str(), UOMAP_MAP0);
        pMapbuffer = new cMapbuffer;
       
        pGroundTextureLoader = new cGroundTextureLoader(mul_texmaps, mul_texidx);
	pTextureBuffer = new TextureBuffer;
	pAnimBuffer = new AnimBuffer;
	
	pArtLoader = new cArtLoader(mulpath+"art.mul", mulpath+"artidx.mul");
	pVerdataLoader = new cVerdataLoader(mulpath+"verdata.mul");
	pAnimLoader = new cAnimLoader(mulpath+"anim.mul", mulpath+"anim.idx");
	pFontLoader = new cFontLoader(mulpath+"fonts.mul");
	pHueLoader = new cHueLoader(mulpath+"hues.mul");

	pTileDataLoader = new cTileDataLoader(mulpath+"tiledata.mul"); 
	pGumpLoader = new cGumpLoader(mulpath+"gumpart.mul", mulpath+"gumpidx.mul");
//	pGrannyLoader = new cGrannyLoader("granny.xml", mulpath);
	pSkillLoader = new cSkillLoader(mulpath+"skills.mul", mulpath+"skills.idx");
}


int Game::Init(void)
{
       

       LoadMuls(nConfig::mulpath);
       
//       pTextManager = new cTextManager;
	   pDynamicObjectList = new cDynamicObjectList;
	   pCharacterList = new cCharacterList; 
       
//       pStaticEngine = new StaticEngine;
//       pStaticEngine->Load("./data/models.dat");
 
       pDebug.Log("SYS | Initializing renderer....");
  	renderer = new Renderer();
  	if(!renderer) {
    		pDebug.Log("Renderer Creation failed in Game::Init(void)", __FILE__,
	       		__LINE__, LEVEL_CRITICAL);
    	return (false);
  	}
 

	if(!renderer->Init()) {
    		pDebug.Log("Renderer Initialization failed in Game::Init(void)", __FILE__,
	       __LINE__, LEVEL_ERROR);
    		return (false);
  	}

/*	SetPosition(nConfig::startx, nConfig::starty, nConfig::startz);	 */
	SetPause (true);

  return (true);
}

int Game::DeInit(void)
{


   
	pDebug.Log("SYS | Unloading UO Files....");
/*    delete pTextManager;
    pTextManager = NULL; 
    */
	delete pMapLoader;
    pMapLoader = NULL;
    delete pMapbuffer;
    pMapbuffer = NULL;
    delete pGroundTextureLoader;
    pGroundTextureLoader = NULL;
    
    delete pAnimBuffer;
    pAnimBuffer = NULL;
    delete pTextureBuffer;
    pTextureBuffer = NULL;
    delete pGumpLoader;
    pGumpLoader = NULL; 
    delete pArtLoader;
    pArtLoader = NULL;
    delete pAnimLoader;
    pAnimLoader = NULL;
    
    delete pVerdataLoader;
    pVerdataLoader = NULL;
    
    delete pFontLoader;
    pFontLoader = NULL; 

    delete pHueLoader;
    pHueLoader = NULL;
    
    delete pSkillLoader;
    pSkillLoader = NULL;

	delete pTileDataLoader;
	pTileDataLoader = NULL;
    
	delete pDynamicObjectList;
	pDynamicObjectList = NULL;

	delete pCharacterList;
	pCharacterList = NULL; 
    

    if (renderer) {
        pDebug.Log("SYS | Shutting down renderer....");
    	if(!renderer->DeInit()) {
      		pDebug.Log("Renderer Shutdown failed in Game::DeInit(void)", __FILE__,
		 	__LINE__, LEVEL_ERROR);
      		return (false);
    	}
       delete renderer;
       renderer = NULL;
    }
       


 return (true); 
}

int Game::RenderScene(void)
{
  if(!renderer) 
    return (false);
  

  return renderer->RenderScene(); 
}

int Game::Handle(void)
{

//renderer->Scroll(0, 2);
//renderer->Scroll(3, 2);
//renderer->Scroll(1, 0);
  
/*  if (pCharacterList)
	  pCharacterList->Handle(time);
*/ 
  if (pMapbuffer && renderer)
  	pMapbuffer->FreeBuffer(renderer->baseblock_x,renderer->baseblock_y, renderer->GetViewDistance() + 5);
/*	
  if (button_right)
  	MoveToMouse(); */

  return (true);
}

Renderer *Game::GetRenderer(void)
{
  return renderer;
}

void Game::OnKeyPress(SDL_keysym * key)
{
}

void Game::HandleMouseMotion(SDL_MouseMotionEvent * event)
{
	if (!event)
		return;
	if (event->state) {
		int dx = - event->xrel;
		int dy = - event->yrel;
		if (dx < -150) dx = -150;
		if (dx > 150) dx = 150;
		if (dy < -150) dy = -150;
		if (dy > 150) dy = 150;
		renderer->Scroll(dx, dy);
	}
}

void Game::HandleClick(int x, int y, unsigned int buttonstate, bool double_click)
{
}

void Game::SetPosition(int x, int y, int z)
{
    if (renderer)
        renderer->TeleportTo(x, y);
}


void game_OnGameStart (void) {
	pGame.SetPause (false);
}

void game_OnTeleport (int x, int y, int z) {
	pGame.SetPosition (x, y, z);
}

void game_OnDragCancel (void) {
	pGame.DragCancel ();
}

void game_OnDrag (unsigned int id, unsigned int model) {
	pGame.Drag (id, model);
}

void Game::Connect ()
{
}

void Game::Disconnect ()
{
}

void Game::Walk (Uint8 direction)
{
/*	if (pClient) 
			pClient->Send_Walk(direction); */
}

void Game::Walk_Simple (Uint8 action)
{
/*	if (pClient) 
			pClient->Walk_Simple(action); */
}

void Game::GrabMousePosition(int x, int y)
{
/*	if (!pCamera || !renderer || paused()) return;

	{
		// Obtain Cam Coords and PickRay

		float vecPickRayOrigin[3];
		float vecPickRayDir[3];
		bool loop = false;

		pCamera->CreatePickRay(x,y,vecPickRayOrigin,vecPickRayDir);

		int CamBlockX,CamBlockY;
		float CamX,CamY,CamZ;

		CamBlockX = pCamera->GetBlockX();
		CamBlockY = pCamera->GetBlockY();
		CamX=vecPickRayOrigin[0] + CamBlockX * 8.0f;
		CamY=vecPickRayOrigin[1] + CamBlockY * 8.0f;
		CamZ=vecPickRayOrigin[2];
		vecPickRayOrigin[2] *= 0.1f;
		vecPickRayDir[2] *= 0.1f;

		CamBlockX = (int) CamX / 8;
		CamBlockY = (int) CamY / 8;
		CamX -= CamBlockX * 8.0f;
		CamY -= CamBlockY * 8.0f;

		// Check Blocks against ray
		
		int view_distance = renderer->GetViewDistance();

		int blockx = pCamera->GetBlockX();
		int blocky = pCamera->GetBlockY();

		sStaticObject * picked_object = NULL;
		int picked_x = -1;
		int picked_y = -1;
		int picked_z = 0;
		float distance = 1000000.0f;

		for (int y = - view_distance; y < view_distance; y++)
  			for (int x = - view_distance; x < view_distance; x++)
			{
  				cMapblock * block = pMapbuffer->CreateBlock(blockx + x, blocky + y);
  				if (block) {
					float act_distance;
					int z;
					sStaticObject * result = block->CheckRay(vecPickRayOrigin, vecPickRayDir, act_distance, x * 8.0f, y * 8.0f);
					if (result)
						if (act_distance < distance) {
							distance = act_distance;
							picked_object = result;
							picked_x = -1;
							picked_y = -1;
						}
					int id = block->CheckRayOnGround(vecPickRayOrigin, vecPickRayDir, act_distance, x * 8.0f, y * 8.0f, z);
					if (id != 255) {
						if (act_distance < distance) {
							distance = act_distance;
							picked_object = NULL;
							picked_x = (id % 8) + (blockx + x) * 8;
							picked_y = (id / 8) + (blocky + y) * 8;
							picked_z = z;
						} 
					}


  				}
			}

			if (picked_object) {
				cursor3d[0] = picked_object->x + picked_object->blockx * 8;
				cursor3d[1] = picked_object->y + picked_object->blocky * 8;
				cursor3d[2] = picked_object->z + picked_object->height;
			}

			if (picked_x != -1) {
				cursor3d[0] = picked_x;
				cursor3d[1] = picked_y;
				cursor3d[2] = picked_z;
			}
		}
*/
}


cDynamicObject * Game::GrabDynamicObject(int x, int y)
{
/*	if (!pCamera || !renderer || paused()) return NULL;

	{
		// Obtain Cam Coords and PickRay

		float vecPickRayOrigin[3];
		float vecPickRayDir[3];
		bool loop = false;

		pCamera->CreatePickRay(x,y,vecPickRayOrigin,vecPickRayDir);

		int CamBlockX,CamBlockY;
		float CamX,CamY,CamZ;

		CamBlockX = pCamera->GetBlockX();
		CamBlockY = pCamera->GetBlockY();
		CamX=vecPickRayOrigin[0] + CamBlockX * 8.0f;
		CamY=vecPickRayOrigin[1] + CamBlockY * 8.0f;
		CamZ=vecPickRayOrigin[2];
		vecPickRayOrigin[2] *= 0.1f;
		vecPickRayDir[2] *= 0.1f;

		CamBlockX = (int) CamX / 8;
		CamBlockY = (int) CamY / 8;
		CamX -= CamBlockX * 8.0f;
		CamY -= CamBlockY * 8.0f;

		
		// Check Blocks against ray
		

		int blockx = pCamera->GetBlockX();
		int blocky = pCamera->GetBlockY();
		int deltax = blockx * 8;
		int deltay = blocky * 8;

		cDynamicObject * picked_object = NULL;
		float distance = 1000000.0f;
		float lambda;
  		dynamiclist_t * dynamics = pDynamicObjectList->GetList();
  		dynamiclist_t::iterator iter;
	
  		for (iter = dynamics->begin(); iter != dynamics->end(); iter++) {
			cDynamicObject * object = iter->second;
			StaticModel * model = pStaticEngine->Get(object->model);
			if (model) {
				if (model->CheckRay(vecPickRayOrigin, vecPickRayDir, object->x - deltax, object->y - deltay, object->z, lambda)) 
					if (lambda < distance) {
						distance = lambda;
						picked_object = object;
					}
			}
		}
		
		return picked_object;
	} */
	return NULL;
 }

 
void Game::HandleMouseDown(int x, int y, int button)
{
/*	if (paused()) return;
	cursorx = x;
	cursory = y;
	GrabMousePosition(cursorx, cursory);
	if (button & (SDL_BUTTON(1)))
		button_left = true;
	if (button & (SDL_BUTTON(2) | SDL_BUTTON(3)))
		button_right = true; */
}


void Game::HandleMouseUp(int x, int y, int button)
{
/*	if (paused()) return;
	cursorx = x;
	cursory = y;
	GrabMousePosition(cursorx, cursory);
	if (button & (SDL_BUTTON(1)))
		button_left = false;
	if (button & (SDL_BUTTON(2) | SDL_BUTTON(3)))
		button_right = false;
	
	if (drag_id) {
		pClient->Send_DropRequest(drag_id, cursor3d[0], cursor3d[1], cursor3d[2]);
	}	
	
	drag_id = 0;
	if (renderer)
		renderer->setDragModel(0); */
}

void Game::MoveToMouse()
{
/*	if (!pClient)
		return;
		
	int px, py, pz;
	pClient->GetPlayerPosition(px, py, pz);
		

	GrabMousePosition(cursorx, cursory);	
	int dx = cursor3d[0] - px;
	int dy = cursor3d[1] - py;
	float angle;
	
	if (dx) {
		float m = (float) dy / (float) dx;
		angle = atan (m) / 3.14159f * 180.0f;	
		if (dx < 0)
			angle = 180.0f + angle;
		if (angle < 0.0f)
			angle += 360.0f;
	} else if (dy > 0) 
		angle = 90.0f;
	 else if (dy < 0) 
		angle = 270.0f;
	angle += 22.5f;
	
	int dir = (int) ((angle + 90.0f) / 45.0f);
	if (dir > 7) dir -= 8;
	
	pClient->Send_Walk(dir);
*/		
}

void Game::HandleDrag(int x, int y)
{
/*	if (paused() || !pClient || !pDynamicObjectList || !pCharacterList) return;
	cDynamicObject * object = GrabDynamicObject(x, y);
	if (object) {
		drag_id = object->id;
		drag_model = object->model;		
		renderer->setDragModel(drag_model, cursor3d[0], cursor3d[1],cursor3d[2]);
		pClient->Send_PickupRequest(object->id);
	} */
}


void Game::DragCancel ()
{
/*	drag_id = 0;
	if (renderer)
		renderer->setDragModel(0); */
}

void Game::Drag (Uint32 id, Uint16 model)
{
/*	if (paused() || !pClient || !pDynamicObjectList || !pCharacterList) return;
	drag_id = id;
	drag_model = model;		
	renderer->setDragModel(drag_model, cursor3d[0], cursor3d[1],cursor3d[2]);
	pClient->Send_PickupRequest(id);  */
}
