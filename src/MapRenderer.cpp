//
// File: MapRenderer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include <fstream>
#include "renderer/SDLScreen.h"
#include "renderer/TextureBuffer.h"
#include "loaders/Map.h"
#include "Debug.h"
#include "Game.h"
#include "Config.h"

using namespace std;


SDLScreen *SDLscreen;

extern "C" // needed by SDL
int main(int argc, char *argv[])
{
  printf("(Re)MapRenderer v2.0 (%zu bits build).\n", (sizeof(void*) * 8));

  if(!nConfig::Init()) {
    pDebug.Log("Unable to load configuration file - Using defaults ", __FILE__, __LINE__,
	       LEVEL_WARNING);
  }

  SDLscreen = new SDLScreen();

  int detail = nConfig::detail;
  if (detail < 0) detail = 0;
  if (detail > 9) detail = 9;
  const int divisor = 1 << detail;
  pGame.Init();
  
  if (nConfig::patches.size()) 
      pMapLoader->Load((char*) nConfig::patches.c_str());
  
  INT_Rect outbox = pGame.GetRenderer()->CalcRenderBox();
  const int renderwidth = outbox.w;
  const int renderheight = outbox.h;  

  const int t_width = ((int) ((renderwidth + divisor * 4 - 1) / (divisor * 4))) * divisor * 4;
  const int t_height = ((int) ((renderheight + divisor * 4 - 1) / (divisor * 4))) * divisor * 4;

  const int rc_width = RENDER_CACHE_WIDTH;
  const int rc_height = RENDER_CACHE_HEIGHT;
  
  SDLscreen->Init(rc_width, rc_height, 32);

  // 24 bpp uncompressed bitmap
  Uint8 bmpheader[54] = {
   0x42,0x4D,0xCE,0x3A,0x00,0x00,0x00,0x00,0x00,0x00,
   0x36,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x64,0x00,
   0x00,0x00,0x32,0x00,0x00,0x00,0x01,0x00,0x18,0x00,
   0x00,0x00,0x00,0x00,0x98,0x3A,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00};
   
  Uint32 * bmpwidth =  (Uint32 *) (bmpheader + 18);
  Uint32 * bmpheight = (Uint32 *) (bmpheader + 22);
  
  *bmpwidth = t_width / divisor;
  *bmpheight = t_height / divisor;
  
  const size_t t_pixels = (size_t)(*bmpwidth) * (*bmpheight);
  const size_t realmem = sizeof(bmpheader) + (t_pixels * 3);  // * 3 -> a pixel has a byte for R, for G, for B
  printf("Creating Bitmap file (%u x %u): %zu bytes (Zoom 1/%i)...", (*bmpwidth), (*bmpheight), realmem, divisor);
  
  Uint8 * data = (Uint8 *) malloc (rc_width * rc_height * 3);
  memset(data, 0, rc_width * rc_height * 3);

  ofstream file(nConfig::output.c_str(), ios::out | ios::binary);
  if (file.fail())
  {
      printf(" error.\n");
      goto goto_end;
  }

  file.write((char*) bmpheader, sizeof(bmpheader));

  const size_t blockcount = t_pixels / (rc_width * rc_height);
  for (size_t i = 0; i < blockcount; ++i)
    file.write((char*) data, rc_width * rc_height * 3);
    
  const size_t rest = t_pixels % (rc_width * rc_height);
  if (rest > 0) 
    file.write((char*) data, rest * 3);
  
  printf(" done.\n");

  printf("Area to render (X/Y): (%i/%i) to (%i/%i).\n", nConfig::minx, nConfig::miny, nConfig::maxx, nConfig::maxy);
  
  const int bxcount = t_width / rc_width;
  const int bycount = t_height / rc_height;
  const int bcount = (bxcount + 1) * (bycount + 1);
  
  const int xsize = rc_width / divisor;
  const int ysize = rc_height / divisor;
  
  int idx = 1;

  constexpr size_t scalebuf_size = (RENDER_CACHE_WIDTH * RENDER_CACHE_HEIGHT * 3)/2;
  Uint8 *scalebuf = nullptr;
  if (detail > 0)
      scalebuf = new Uint8[scalebuf_size];
  for (int blocky = 0; blocky <= bycount; ++blocky)
     for (int blockx = 0; blockx <= bxcount; ++blockx)
     {
       int xcount = xsize; if (blockx == bxcount) xcount = (t_width % rc_width) / divisor;
       int ycount = ysize; if (blocky == bycount) ycount = (t_height % rc_height) / divisor;

       printf("\rRendering Block %i/%i...", idx, bcount);

       // Render map block in memory (in a temporary SDL screen of size RENDER_CACHE_WIDTH x RENDER_CACHE_HEIGHT)
       SDL_FillRect(SDLscreen->screen, NULL, 0);
       pGame.GetRenderer()->Rebuild(- blockx * rc_width,- blocky * rc_height);
       
       Uint8 * pixels = (Uint8 *) SDLscreen->screen->pixels;
       // Adjust detail of the screen, if detail > 0
       if (detail > 0)
       {
           //memset(scalebuf, 0, scalebuf_size);
           int w = rc_width;
           int h = rc_height;

           // Scale by 2 for each detail point (total effect is a scale of 1 : pow(2, detail))
           for (int i = 0; i < detail; ++i)
           {
               w /= 2; h /= 2;
               //Uint8 * scalebuf = (Uint8 *) malloc (w * h * 3);
               for (int y = 0; y < h; ++y)
               {
                   const Uint8 * src = pixels + y * w * 12;
                   Uint8 * dst = scalebuf + y * w * 3;
                   for (int x = 0; x < w; ++x)
                   {
                       *dst = (*src + *(src + 3) + *(src + w * 6) + *(src + w * 6 + 3)) / 4;
                       *(dst+1) = (*(src+1) + *(src + 4) + *(src + w * 6 + 1) + *(src + w * 6 + 4)) / 4;
                       *(dst+2) = (*(src+2) + *(src + 5) + *(src + w * 6 + 2) + *(src + w * 6 + 5)) / 4;
                       src += 6; dst += 3;
                   }
               }
               pixels = scalebuf;
           }
       }
  
       // Write to file the rendered block
       for (int y = 0; y < ycount; ++y)
       {
           // Swap RGB bytes to BGR (24 bpp bitmap stores the color bytes in this order)
           const Uint8 * src = (pixels)+ y * rc_width / divisor * 3;
           Uint8 * dst = data;
           for (int x = 0; x < xcount; ++x)
           {
               *dst = *(src + 2);
               *(dst + 1) = *(src + 1);
               *(dst + 2) = *src;
               dst += 3; src += 3;
           }

           file.seekp(sizeof(bmpheader) + (t_width / divisor * (t_height / divisor - 1 - blocky * ysize - y) + blockx * xsize)* 3, ios::beg);
           file.write((char *)data, xcount * 3);
       }
                    
       ++idx;
  }
  if (detail > 0) 
      delete[] scalebuf; 
  printf("\nDone!\n");

goto_end:
   free(data);
  
  pGame.DeInit();
  delete SDLscreen;

  if (argc)
  {
    printf("Press a key to exit...\n.");
    getchar();
  }

  return 0;
}

