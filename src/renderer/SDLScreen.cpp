//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//

#include "renderer/SDLScreen.h"
#include "Config.h"
#include "loaders/FontLoader.h"


int flag_cullbackfaces = 0;
int texture_mem = 0;

float BilinearTable[32][32][4];

void CalcBilinearTable ()
{   
    int x, y; 
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
/*  float d4 = fracx * fracy;
  float d3 = (1.0f - fracx) * fracy;
  float d2 = fracx * (1.0f - fracy);
  float d1 = (1.0f - fracx) * (1.0f - fracy);

/*	float frac = sqrt((fracx * fracx + fracy * fracy) / 2); */

//  float smooth_z = (d1 * z1 + d2 * z2 + d3 * z3 + d4 * z4);

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
  ClearFonts();
  SDL_Quit();
}

int
  SDLScreen::Init(int width, int height, int bpp)
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
  

/*  atexit(SDLNet_Quit);
  atexit(NET2_Quit);
  atexit(SDL_Quit); */
  //atexit(TTF_Quit);
  //SDL_ShowCursor( SDL_DISABLE );
  //SDL_WarpMouse( 320, 240 );
  //NET2_Init();

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

void SDLScreen::DisplayFps()
{
  if (!screen)
  	return;
  static float framesPerSecond = 0.0f;	// This will store our fps
  static float lastTime = 0.0f;	// This will hold the time from the last frame
  static char strFrameRate[40] = { "Fps: " };

  float currentTime = SDL_GetTicks() * 0.001f;

  ++framesPerSecond;
  if(currentTime - lastTime > 1) {
    lastTime = currentTime;
    // Copy the frames per second into a string to display in the window title bar
    sprintf(strFrameRate, "Ultima Iris, fps: %d", int (framesPerSecond));

    // Set the window title bar to our string
    SDL_WM_SetCaption(strFrameRate, NULL);	// The First argument is the window title
    // Reset the frames per second
    framesPerSecond = 0;

  };

  //string fps = string(strFrameRate);
  //UOfont->drawString(0, fps, 0 ,0); */
}

int SDLScreen::ScreenSave()
{
/*  if (!screen)
  	return 1;
#ifndef WIN32
  int screenInfo[4];

  glGetIntegerv(GL_VIEWPORT, screenInfo);

  unsigned char *image;
  image = new unsigned char[screenInfo[2] * screenInfo[3] * 3];

  glReadPixels(0, 0, screenInfo[2], screenInfo[3], GL_BGR,
	       GL_UNSIGNED_BYTE, image);

  ofstream screenshot;

  screenshot.open("screenshot.tga", ios::out | ios::binary);

  if(!screenshot.is_open()) {
    cerr << "Could not open file writing\n";
    return 0;
  };

  unsigned char Fileheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned char Imageheader[6] = { (screenInfo[2] % 256),
    (screenInfo[2] / 256),
    (screenInfo[3] % 256),
    (screenInfo[3] / 256), 24, 0
  };

  screenshot.write((char *) &Fileheader, sizeof(Fileheader));
  screenshot.write((char *) &Imageheader, sizeof(Imageheader));
  screenshot.write((char *) image, screenInfo[2] * screenInfo[3] * 3);

  screenshot.close();
  delete[]image;
#endif */

  return 1;
}



void SDLScreen::SetPerspective()
{
}

float SDLScreen::GetRatio(void)
{
	return 1.0f;
}

void SDLScreen::SetAlpha(Uint8 alpha)
{
}

void SDLScreen::SetHue(Uint16 hue)
{

}

void SDLScreen::SetLight(float factor)
{
}

void SDLScreen::Flip()
{
	if (screen)
		SDL_Flip(screen);
	DisplayFps();
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

void SDLScreen::Lock()
{
	if (screen)
		SDL_LockSurface(screen);
}

void SDLScreen::Unlock()
{
	if (screen)
		SDL_UnlockSurface(screen);
}

void SDLScreen::ClearFonts ()
{
}

void SDLScreen::RegisterFont (Uint32 id, std::string filename, Uint32 size)
{
}

void SDLScreen::UnregisterFont (Uint32 id)
{
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
}

