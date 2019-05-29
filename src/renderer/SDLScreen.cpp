//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//

#include "renderer/SDLScreen.h"
#include "Config.h"


int flag_cullbackfaces = 0;
int texture_mem = 0;

float BilinearTable[32][32][4];

void CalcBilinearTable ()
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
  do_redraw = false;
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
  if(nConfig::startfullscreen)
    videoFlags |= SDL_FULLSCREEN;

  //videoFlags |= SDL_RESIZABLE;        /* Enable window resizing */

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

  	/* Enable key repeat */
  	if((SDL_EnableKeyRepeat
      		(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL))) {
    		cerr << "Setting keyboard repeat failed: " << SDL_GetError() << endl;
    		exit(1);
  	}
	

  SDL_EnableUNICODE(1);
  
  return true;
}


void SDLScreen::ToggleFullScreen()
{
  if (!screen)
  	return;

  SDL_WM_ToggleFullScreen(screen);
}

void SDLScreen::ClearScreen()
{
  if (!screen)
  	return;
  SDL_FillRect(screen, NULL, 0);
  /* Clear The Screen And The Depth Buffer */
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLScreen::ClearZBuffer()
{
  if (!screen)
  	return;
  /* Clear The Depth Buffer */
  //glClear(GL_DEPTH_BUFFER_BIT);
}

/* Blit the Screen*/

int SDLScreen::DrawGL(void)
{
/*  if (!screen)
  	return true;
  DisplayFps();

   glFinish();
 SDL_GL_SwapBuffers();
//  glSwapBuffers();
*/
  return (true);
}

void SDLScreen::DrawSurface (SDL_Surface * surface, int x, int y)
{
	if (!surface)
		return;
		
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = surface->w;
	rect.h = surface->h;
	if (screen)
		SDL_BlitSurface(surface, NULL, screen, &rect);
}

void SDLScreen::CheckUpdate()
  {
    act_update = SDL_GetTicks();
    list<Uint32>::iterator iter;
    for (iter = update_times.begin(); iter != update_times.end(); iter++) {
        if (*iter > act_update) {
//                do_redraw = true;
                update_times.erase(iter);
                iter = update_times.begin();
        }
    }
  }
  
void SDLScreen::DoUpdate (Uint32 time)
  {
    update_times.push_back(act_update + time);
  } 

int SDLScreen::GetTextWidth(char * text, int font)
{
    return 0; // NOTE: INCOMPLETE CODE!
}

