//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//

#include "renderer/SDLScreen.h"
#include "Config.h"


int texture_mem = 0;

float BilinearTable[32][32][4];

static void CalcBilinearTable ()
{   
    float fracx, fracy;
    for (int y = 0; y < 32; y++)
        for (int x = 0; x < 32; x++)
           {
                      fracx = (float) x / 32.0f;
                      fracy = (float) y / 32.0f;
                      BilinearTable[y][x][0] = fracx * fracy;
                      BilinearTable[y][x][1] = (1.0f - fracx) * fracy;
                      BilinearTable[y][x][2] = fracx * (1.0f - fracy);
                      BilinearTable[y][x][3] = (1.0f - fracx) * (1.0f - fracy);
           }
}

SDLScreen::SDLScreen()
{
  act_alpha = 255;
  light_factor = 1.0f;
  screen = NULL;
  act_update = SDL_GetTicks();
//  nConfig::width = (nConfig::maxblockx - nConfig::minblockx + 2) * 176 * 2;
//  nConfig::height = (nConfig::maxblocky - nConfig::minblocky + 2) * 176 * 2;
//  Init(nConfig::width, nConfig::height, nConfig::bpp);
  CalcBilinearTable();
}

SDLScreen::~SDLScreen()
{
  if (screen)
  	SDL_FreeSurface(screen);
  SDL_Quit();
}

int SDLScreen::Init(int width, int height, int bpp)
{

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);


  /* Fetch the video info */
  videoInfo = SDL_GetVideoInfo();

  if(!videoInfo) {
    cerr << "Video query failed: " << SDL_GetError();
    return (1);
  }

  /* the flags to pass to SDL_SetVideoMode */
  videoFlags = 0;	/* Enable OpenGL in SDL */
//  videoFlags |= SDL_GL_DOUBLEBUFFER;	/* Enable double buffering */
  videoFlags |= SDL_HWPALETTE;	/* Store the palette in hardware */

  /* This checks to see if surfaces can be stored in memory */
  if(videoInfo->hw_available) {
    videoFlags |= SDL_HWSURFACE;
  } else {
    videoFlags |= SDL_SWSURFACE;
  };

  /* This checks if hardware blits can be done */
  if(videoInfo->blit_hw) {
    videoFlags |= SDL_HWACCEL;
  }

  /* Sets up OpenGL double buffering */
  //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Set name of application
//  SDL_WM_SetCaption("Ultima Iris 2D", NULL);

	  /* get a SDL surface */
	  screen = SDL_CreateRGBSurface(SDL_SWSURFACE , width, height, 24, SDL_RGB_BITMASK);
//  	screen = SDL_CreateRGBSurface(width, height, 16, videoFlags);
  	/* Verify there is a surface */
  	if(!screen) {
    		cerr << "Video mode set failed: " << SDL_GetError() << endl;
    		exit(1);
  	}	
  
  return true;
}

