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
#include "loaders/VerdataLoader.h"
#include "loaders/ArtLoader.h"
#include "loaders/HueLoader.h"
#include "loaders/TileDataLoader.h"

#include "renderer/MapBuffer.h"
#include "renderer/TextureBuffer.h"

using namespace std;

Game pGame;

extern SDLScreen *SDLscreen;



Game::Game()
{
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
	
	string mul_map =       mulpath+ "map"    + to_string(nConfig::mapindex) + ".mul";
	string mul_statics =   mulpath+ "statics"+ to_string(nConfig::mapindex) + ".mul";
	string mul_staidx =    mulpath+ "staidx" + to_string(nConfig::mapindex) + ".mul";
	string mul_texmaps =   mulpath+ "texmaps.mul";
	string mul_texidx =    mulpath+ "texidx.mul";
       
        pDebug.Log("SYS | Loading UO Files....");	
        pMapLoader = new UOMapLoader(mul_map.c_str(), mul_statics.c_str(), mul_staidx.c_str(), nConfig::widthblocks, nConfig::heightblocks);
        pMapbuffer = new cMapbuffer;
       
        pGroundTextureLoader = new cGroundTextureLoader(mul_texmaps, mul_texidx);
	pTextureBuffer = new TextureBuffer;
	
	pArtLoader = new cArtLoader(mulpath+"art.mul", mulpath+"artidx.mul");
	pVerdataLoader = new cVerdataLoader(mulpath+"verdata.mul");
	pHueLoader = new cHueLoader(mulpath+"hues.mul");

	pTileDataLoader = new cTileDataLoader(mulpath+"tiledata.mul"); 
}


int Game::Init(void)
{
       LoadMuls(nConfig::mulpath);
        
       pDebug.Log("SYS | Initializing renderer....");
  	renderer = new Renderer();
  	if(!renderer) {
    		pDebug.Log("Renderer Creation failed in Game::Init(void)", __FILE__,
	       		__LINE__, LEVEL_CRITICAL);
    	return (false);
  	}

  return (true);
}

int Game::DeInit(void)
{
	pDebug.Log("SYS | Unloading UO Files....");

	delete pMapLoader;
    pMapLoader = NULL;
    delete pMapbuffer;
    pMapbuffer = NULL;
    delete pGroundTextureLoader;
    pGroundTextureLoader = NULL;
    
    delete pTextureBuffer;
    pTextureBuffer = NULL;
    delete pArtLoader;
    pArtLoader = NULL;
    
    delete pVerdataLoader;
    pVerdataLoader = NULL;
    
    delete pHueLoader;
    pHueLoader = NULL;

	delete pTileDataLoader;
	pTileDataLoader = NULL;
    
    if (renderer) {
       delete renderer;
       renderer = NULL;
    }

 return (true); 
}

Renderer *Game::GetRenderer(void)
{
  return renderer;
}
