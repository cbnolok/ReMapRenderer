//
// File: MapBlock.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "renderer/MapBlock.h"
#include "Debug.h"
#include "include.h"
#include "math.h"
#include "Config.h"
#include "uotype.h"
#include "loaders/Map.h"
#include "renderer/TextureBuffer.h"
#include "renderer/StaticObjects.h"
#include "renderer/SDLScreen.h"
#include "renderer/AnimBuffer.h"
#include "loaders/Statics.h"
#include "Geometry.h"
#include "loaders/TileDataLoader.h"
#include "loaders/HueLoader.h"

using namespace std;

extern SDLScreen *SDLscreen;

Uint32
getAnimationBase (Uint32 model)
{
  if (model < 0xc8)
    {
      return model * 110;
    }

  if (model < 0x190)
    {
      return (model - 0x0C8) * 65 + 22000;
    }

  return (model - 0x190) * 175 + 35000;
}

void PutPixel(SDL_Surface * screen, Sint32 X, Sint32 Y, int len, SDL_Surface * texture, float m_x, float m_y, float tex_y)
{
    /* Code to set a yellow pixel at the center of the screen */

    Uint32   pixel;
    Uint8   *bits, bpp;

    /* Map the color yellow to this display (R=0xFF, G=0xFF, B=0x00)
       Note:  If the display is palettized, you must set the palette first.
    */
    Uint32 op = 63;
    if (texture->h == 128)
    	op = 127;
    Uint32 * p = 0;
    Uint32 * q;
    p = (Uint32 *) texture->pixels;
     
    float tex_x = 0.0f;
    pixel = 0xff000000;//rand();//SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

    /* Calculate the framebuffer offset of the center of the screen */
    bpp = screen->format->BytesPerPixel;
    bits = ((Uint8 *)screen->pixels)+Y*screen->pitch+X*bpp;
    //if (p)
	//printf("%x\n", *p);
    while (len >= 0) {
    	q = p + (((int) (tex_y * (float) texture->h)) & op) * texture->w;
  	*((Uint32 *)(bits)) = *(q + ((int) ((float) texture->w * tex_x)& op) ) | pixel;//(Uint32)pixel;
	
	tex_x += m_x;
	tex_y -= m_y;
	bits += 4;
	len --;
    } 

    /* Update the display */
    //SDL_UpdateRect(screen, X, Y, 1, abs(len));

    return;
   }
   
 void DrawHalfUpperTriangle(SDL_Surface * screen, int * up, float m1, float m2, int lines, SDL_Surface * texture, float t) 
 {
 	int i, j;
	
	
	float p[2], q[2];
	for (i=0; i < 2; i++) {
		p[i] = (float) up[i];
		q[i] = p[i];
	}
	for (i = 0; i < lines; i++) {
		float tex_y = (float) i / (float) lines;
		float tex1_x = 0.0f;
		float tex2_x = tex_y * t;
	        int len = (int) (q[0] - p[0]);
		if (len > 0) {
			j = (int) p[0]; 
		} else { j = (int)q[0]; }
		float m_x =  0.0f;
		if (len)
			m_x = tex2_x / ((float) len * t);
		float m_y = 0.0f;
		m_y = (1.0f - t) / (float)(lines);
//		printf("%.2f\n", m_y);
		PutPixel(screen, j, up[1] + i,abs(len), texture, m_x, m_y, tex_y);
		p[0] += m1;
		q[0] += m2;
	}
 }

 void DrawHalfLowerTriangle(SDL_Surface * screen, int * up, float m1, float m2, int lines, SDL_Surface * texture, float t) 
 {
 	int i, j;
	float p[2], q[2];
	for (i=0; i < 2; i++) {
		p[i] = (float) up[i];
		q[i] = p[i];
	}
	for (i = 0; i < lines; i++) {
		float tex_y = (float) i / (float) lines;
		float tex1_x = 0.0f;
		float tex2_x = tex_y;
	        int len = (int) (q[0] - p[0]);
		if (len > 0)
			j = (int) p[0]; else j = (int)q[0];
		float m_x =  0.0f;
		if (len)
			m_x = tex2_x / ((float) len * t);
		float m_y = 0.0f;
		m_y = (1.0f - t) / (float)(lines);
		PutPixel(screen, j, up[1] - i,abs(len), texture, m_x, m_y, tex_y);
		p[0] += m1;
		q[0] += m2;
	}
 }

 void DrawTriangle(SDL_Surface * screen, int * p1, int * p2, int *p3, SDL_Surface * texture) 
 {
 	
 	int * upper, *middle, *lower;
	if ((p1[1] < p2[1]) && (p1[1] < p3[1]))
	 {
		upper = p1;
		if (p2[1] < p3[1]) {
			middle = p2;
			lower = p3;
		} else {
			middle = p3;
			lower = p2;
		}
	 }
	else
	if ((p2[1] < p1[1]) && (p2[1] < p3[1])) {
		upper = p2;
		if (p1[1] < p3[1]) {
			middle = p1;
			lower = p3;
		} else {
			middle = p3;
			lower = p1;
		}
	} else {
		upper = p3;
		if (p1[1] < p2[1]) {
			middle = p1;
			lower = p2;
		} else {
			middle = p2;
			lower = p1;
		}
	}
	if ((upper[1] != middle[1]) && (lower[1] != upper[1]))  {
		float m1 = (float)(upper[0] - middle[0]) / (float) (upper[1] - middle[1]);
		float m2 = (float)(upper[0] - lower[0]) / (float) (upper[1] - lower[1]);
		float t = (float)(upper[1] - middle[1]) / (float) (upper[1] - lower[1]);
		DrawHalfUpperTriangle(screen, upper, m2, m1, middle[1] - upper[1], texture, t);
	}
	if ((lower[1] != middle[1]) && (lower[1] != upper[1]))  {
		float m1 = (float)(lower[0] - middle[0]) / (float) (lower[1] - middle[1]);
		float m2 = (float)(lower[0] - upper[0]) / (float) (lower[1] - upper[1]);
		float t = (float)(middle[1] - lower[1]) / (float) (upper[1] - lower[1]);
		DrawHalfLowerTriangle(screen, lower, -m2, -m1, lower[1] - middle[1] + 1, texture, t);
	}
 }

SDL_Surface * CreateHuedSurface(SDL_Surface * surface, Uint16 HueID)
{
    if (!surface)
        return NULL;

    const struct stHue * hue = pHueLoader->getHue(HueID);
    

    SDL_Surface * result = SDL_CreateRGBSurface(SDL_SRCCOLORKEY, surface->w, surface->h, 16, SDL_RGB_16_BITMASK);
    SDL_SetColorKey(result, SDL_SRCCOLORKEY, 0);
    
    Uint16 * src = (Uint16*) surface->pixels;
    Uint16 * dst = (Uint16*) result->pixels;
    int size = surface->w * surface->h;
     
    
    for (int i = 0; i < size; i++) {
        if (*src) 
                *dst = hue->colors[(*src >> 10) & 31];
        src ++; dst ++;
    }
    return result;
}

void LightenPixel(Uint32 * pixel, Sint16 value)
{
    Uint8 * p = (Uint8 *) pixel;
    for (int i = 0; i < 3; i++) {
        Sint16 v = *p + value;
        if (v < 0) v = 0;
        else
          if (v > 255) v = 255;
        *p = v;        
        p++;
    }   
}

int CalcStretchHeight(int y2, int y3, int y4, int & min_y)
{
int max_y = 0;
min_y = 0;

if (y2 < min_y) min_y = y2;
if (y2 > max_y) max_y = y2;
if (y3 < min_y) min_y = y3;
if (y3 > max_y) max_y = y3;
if (y4 < min_y) min_y = y4;
if (y4 > max_y) max_y = y4;

return max_y - min_y + 1;
}

//SDL_Surface * GetStretchedSurface (int *p1, int *p2, int *p3, int *p4, int * hotspot, SDL_Surface * texture)
SDL_Surface * GetStretchedSurface (int y2, int y3, int y4, int * alphas, SDL_Surface * texture, int h, int min_y)
{
if (!texture)
    return NULL;
    
int tsize = texture->w - 1;
float tfac = (float) texture->w / 22.0f;

//y2 = 0; y3 = 0; y4 = 0;

int up_point = y2;
int down_point = y3;
int right_point = y4;

if (up_point > down_point) {
 return NULL;
} 

if (!h) {
    return NULL;
}

//hotspot[0] = 0;
//hotspot[1] = -y2 + min_y;// - 22;

SDL_Surface * surface = SDL_CreateRGBSurface(SDL_SWSURFACE , 44, h, 32, SDL_BITMASK);
//return surface;
float m1 = (float) up_point / 22.0f;
float m2 = (float) down_point / 22.0f;

float yp1 = 0.0f;
float yp2 = 0.0f;

//printf("%i\n", alphas[0]);

Uint32 * q = (Uint32 *) texture->pixels;

for (int x = 0; x < 22; x++) {
    float tx = 0.0f;    
    float ty = x * tfac;
    
    Uint32 * p = (Uint32 *) surface->pixels + x + ((int) yp1 - min_y) * 44;
    float dy = yp2 - yp1 + 1;
    if (!dy) dy = 0.0001f;
    float m = ty / dy;
    
//        printf("%.2f %.2f\n", tx, ty);
    for (int y = (int) yp1; y <= (int) yp2; y++) {
        Uint32 *src = q + (int) tx + (tsize - (int) ty) * texture->w;
 //       printf
        *p = *src;
  //      *p = 0xff8080ff;
        float * table = BilinearTable[(tsize - (int) tx) * 32 / texture->w][((int) ty) * 32 / texture->w];
//        float * table = BilinearTable[(tsize - (int) ty) * 32 / texture->w][(int) tx * 32 / texture->w];
        float alpha = table[0] * alphas[0] + table[1] * alphas[1] + table[2] * alphas[2] + table[3] * alphas[3];
       LightenPixel(p, (int) alpha);
        p += 44;
        tx += m; ty -= m;
    } 
    yp1 += m1;
    yp2 += m2;
}
m1 = (float) (up_point - right_point) / 22.0f;
m2 = (float) (down_point - right_point) / 22.0f;
for (int x = 22; x < 44; x++) {
    float tx = (x - 22) * tfac;    
    float ty = tsize - 1;

    float dy = yp2 - yp1 + 1.0f;
    float m = (ty - tx) / dy;

    Uint32 * p = (Uint32 *) surface->pixels + x + ((int) yp1 - min_y) * 44;
    for (int y = (int) yp1; y <= (int) yp2; y++) {
        Uint32 *src = q + (int) tx + (tsize - (int) ty) * texture->w;
        *p = *src;
//        *p = 0xff8080ff;
        float * table = BilinearTable[(tsize - (int) tx) * 32 / texture->w][((int) ty) * 32 / texture->w];
        float alpha = table[0] * alphas[0] + table[1] * alphas[1] + table[2] * alphas[2] + table[3] * alphas[3];
//        float alpha = table[0] * alphas[3] + table[1] * alphas[2] + table[2] * alphas[1] + table[3] * alphas[0];
        LightenPixel(p, (int) alpha);
        p += 44;
        tx += m; ty -= m;
    } 
    yp1 -= m1;
    yp2 -= m2;
}
return surface;
/*	int min[2], max[2], q1[2], q2[2], q3[2], q4[2];
	int i;
	for (i = 0; i < 2; i++) {
		min[i] = 1000000; max[i] = -1000000;
		if (p1[i] < min[i]) min[i] = p1[i];
		if (p1[i] > max[i]) max[i] = p1[i];
		if (p2[i] < min[i]) min[i] = p2[i];
		if (p2[i] > max[i]) max[i] = p2[i];
		if (p3[i] < min[i]) min[i] = p3[i];
		if (p3[i] > max[i]) max[i] = p3[i];
		if (p4[i] < min[i]) min[i] = p4[i];
		if (p4[i] > max[i]) max[i] = p4[i];
	}
	int w = max[0] - min[0] + 2;
	int h = max[1] - min[1] + 2;
	if (h > 120) {
	}
	SDL_Surface * surface = SDL_CreateRGBSurface(SDL_SWSURFACE , w, h, 32, SDL_BITMASK);
	for (i = 0; i < 2; i++) {
		q1[i] = p1[i] - min[i] + 1;
		q2[i] = p2[i] - min[i] + 1;
		q3[i] = p3[i] - min[i] + 1;
		q4[i] = p4[i] - min[i] + 1;
	}
	DrawTriangle(surface, q1, q2, q3, texture);
	DrawTriangle(surface, q3, q4, q1, texture);
	for (i = 0; i < 2; i++)
		hotspot[i] = -q1[i];
	return surface;*/
	return NULL;
}


cMapblock::cMapblock()
{
  blockx = 0;
  blocky = 0;
  memset(heightmap, 0, sizeof heightmap);
  memset(groundids, 0, sizeof groundids);
  int i, j;
  for (i = 0; i < 8; i++)
	for (j = 0; j < 8; j++) {
		custom_stretched[i][j] = NULL;
		ground_draw_pos[i][j][0] = 0;
		ground_draw_pos[i][j][1] = 0;
	}
}

cMapblock::~cMapblock()
{
  int i, j;
  for (i = 0; i < 8; i++)
	for (j = 0; j < 8; j++) 
		if (custom_stretched[i][j]) 
			SDL_FreeSurface(custom_stretched[i][j]);
}


void CreateObject(cEntity *object, unsigned int x, unsigned int y,
		  int z, unsigned int tileid)
{
  object->x = x % 8;
  object->y = y % 8;
  object->z = z;
  object->blockx = x / 8;
  object->blocky = y / 8;
  object->tileid = tileid;
  object->hue = 0;
  if (pTileDataLoader) {
  	struct TileDataStaticEntry entry;
	pTileDataLoader->LoadEntry(tileid, &entry);
	object->height = entry.height;
	object->tiledata_flags = entry.flags;
	object->tileclass = TILE_CLASS_ITEM;
  }
  if (pTextureBuffer) {
		Texture * texture = pTextureBuffer->GetArtTexture(tileid + 0x4000);
		if (texture) {
			object->w = texture->GetWidth();
			object->h = texture->GetHeight();
		}
			
  }
/*  object->flags = model->modelflags;
  object->height = model->infos.height;
  object->tiledata_flags = model->infos.flags;
  object->sphere[0] = model->boundaring_sphere[0] + object->x;
  object->sphere[1] = model->boundaring_sphere[1] + object->y;
  object->sphere[2] = model->boundaring_sphere[2] + object->z;
  object->sphere[3] = model->boundaring_sphere[3];
  object->sphere[4] = model->boundaring_sphere[4];
  object->alpha = 255;
  object->fader = NULL; */
}

void ExtractCell (struct MulCell * cell, Sint8 * heightmap, Uint16 * groundid) {
	if (heightmap)
		*heightmap = cell->z;
	if (groundid)
		*groundid = cell->tileid;
}

bool cMapblock::Generate(int blockx, int blocky  )
{
  int i, j;
  if (!pMapLoader || !pTextureBuffer)
  	return false;
  struct MulBlock block;

  this->blockx = blockx;
  this->blocky = blocky;


  int x, y;
  
  pMapLoader->LoadMapBlock(blockx, blocky, &block);

  for (y = 0; y < 8; y++)
    for (x = 0; x < 8; x++) 
      ExtractCell(&block.cells[y][x], &heightmap[y][x], &groundids[y][x]);
    

  pMapLoader->LoadMapBlock(blockx + 1, blocky, &block);
  for (y = 0; y < 8; y++)
    for (x = 0; x < 2; x++)
      ExtractCell(&block.cells[y][x], &heightmap[y][8 + x], NULL);

  pMapLoader->LoadMapBlock(blockx, blocky + 1, &block);
  for (y = 0; y < 2; y++)
    for (x = 0; x < 8; x++)
      ExtractCell(&block.cells[y][x], &heightmap[8+y][x], NULL);

  pMapLoader->LoadMapBlock(blockx + 1, blocky + 1, &block);
  for (y = 0; y < 2; y++)
    for (x = 0; x < 2; x++)
  ExtractCell(&block.cells[y][x], &heightmap[8+y][8+x], NULL); 



  int min_x = 1000000;
  int max_x = -1000000;
  int min_y = 1000000;
  int max_y = -1000000;
  
  
  for (i = 0; i < 10; i++)
	for (j = 0; j < 10; j++) {
		int z = ((Sint32) heightmap[i][j]) *4;
		int d_x = ((7-i)+j) *22;
		int d_y = y +(j-(7-i))*22 - z - 44;
		ground_draw_pos[i][j][0] = d_x;
		ground_draw_pos[i][j][1] = d_y;
		if (i < 8 && j < 8) {
		
		}
  }
  
  for (i = 0; i < 9; i++)
	for (j = 0; j < 9; j++) {
	   int y2 = ground_draw_pos[i][j][1] - ground_draw_pos[i+1][j][1];
	   int y3 = ground_draw_pos[i+1][j+1][1] - ground_draw_pos[i+1][j][1];
	   alpha_values[i][j] = 30000.0f / (float) (y2 * y2 + y3 * y3 + 1) - 60.0f;
       if (alpha_values[i][j] > 10.0f) alpha_values[i][j] = 10.0f;
       if (alpha_values[i][j] < -50.0f) alpha_values[i][j] = -50.0f;
  }

  for (i = 0; i < 8; i++)
	for (j = 0; j < 8; j++) {
		cGroundObject * object = objects.AddGround();
		object->x = j;
		object->y = i;
		object->z = heightmap[i][j];
		object->blockx = blockx;
		object->blocky = blocky;
		object->tileid = groundids[i][j];
		object->draw_x = ground_draw_pos[i][j][0];
		object->draw_y = ground_draw_pos[i][j][1];
		object->stretch = 1;
		object->hue = 0;
		
/*	int level = heightmap[i][j];
	Texture * texture = pTextureBuffer->GetGroundTexmap(groundids[i][j]);
	if ((heightmap[i][j+1] != level) || (heightmap[i+1][j+1] != level) || (heightmap[i+1][j] != level) || true) {
		if (texture && texture->GetSurface()) {
//		   printf("----\n");
//		   printf("%i %i | %i %i | %i %i | %i %i\n", ground_draw_pos[i][j][0], ground_draw_pos[i][j][1], ground_draw_pos[i+1][j][0], ground_draw_pos[i+1][j][1], ground_draw_pos[i][j+1][0], ground_draw_pos[i][j+1][1], ground_draw_pos[i+1][j+1][0], ground_draw_pos[i+1][j+1][1]);
//			object->surface = GetStretchedSurface(ground_draw_pos[i][j], ground_draw_pos[i][j+1],ground_draw_pos[i+1][j+1], ground_draw_pos[i+1][j], hotspots[i][j], texture->GetSurface());
//			object->surface = GetStretchedSurface(heightmap[i][j] - heightmap[i+1][j],heightmap[i+1][j+1] - heightmap[i+1][j],heightmap[i][j+1]- heightmap[i+1][j], hotspots[i][j], texture->GetSurface());
            int alphas[4];
            alphas[0] = (int)alpha_values[i][j];
            alphas[1] = (int)alpha_values[i+1][j];
            alphas[2] = (int)alpha_values[i][j+1];
            alphas[3] = (int)alpha_values[i+1][j+1];
            
//			object->surface = GetStretchedSurface(ground_draw_pos[i][j][1] - ground_draw_pos[i+1][j][1],ground_draw_pos[i+1][j+1][1] - ground_draw_pos[i+1][j][1],ground_draw_pos[i][j+1][1] - ground_draw_pos[i+1][j][1], hotspots[i][j], alphas, texture->GetSurface());
			//custom_stretched[i][j] = texture->GetSurface();
			} */
	int w = 44; 
	int h = 44; 
		if (object->draw_x < min_x) 
			min_x = object->draw_x;
		if (object->draw_y < min_y) 
			min_y = object->draw_y;
		if (object->draw_x + w > max_x) 
			max_x = object->draw_x + w;
		if (object->draw_y + h > max_y) 
			max_y = object->draw_y + h; 
	
  }
  
  int staticcount = 0;
  struct staticinfo * statics = pMapLoader->LoadStatics(blockx, blocky, staticcount);
  struct staticinfo * statics_p = statics;
  x = blockx * 8; y = blocky * 8;
  
     for (i = 0; i < staticcount; i++)
  	{
			cEntity * object;
   			if (((statics_p->TileID >= 6038) && (statics_p->TileID <= 6066))) {
    			object = objects.AddGround();
				CreateObject(object, x + statics_p->x, y + statics_p->y, 
					statics_p->z, statics_p->TileID);
				object->tileclass = TILE_CLASS_GROUND;
				object->tileid = statics_p->TileID + 0x4000;
				((cGroundObject *) object)->stretch = 0;
            } else {
    			object = objects.AddStatic();
				CreateObject(object, x + statics_p->x, y + statics_p->y, 
					statics_p->z, statics_p->TileID);
				object->hue = statics_p->Hue;
			}
			int tx = 7-object->y % 8;
			int ty = (object->x % 8);
			int z = (object->z) * 4;
			object->draw_x = (tx+ty) * 22;
			object->draw_y = (ty-tx) * 22 - z - object->h;
			if (object->draw_x < min_x) 
				min_x = object->draw_x;
			if (object->draw_y < min_y) 
				min_y = object->draw_y;
			if (object->draw_x + object->w > max_x) 
				max_x = object->draw_x + object->w;
			if (object->draw_y + object->h > max_y) 
				max_y = object->draw_y + object->h;
			
		
		statics_p ++;
	} 
	
  outbox.x = min_x;
  outbox.y = min_y;
  outbox.w = max_x - min_x;
  outbox.h = max_y - min_y;
  
	
  free(statics); 
  
  objects.Sort();
  
  return true;
}


void cMapblock::RenderGround(int x, int y, SDL_Surface * target, SDL_Rect * cliprect)
{
	if (!pTextureBuffer || !target)
		return;
		if (((outbox.x + outbox.w + x) < 0) || ((outbox.y + outbox.h + y) < 0) || (outbox.x + x > 1023) || (outbox.y + y> 1023))
			return; 
		
	int i, j;
	objectlist_t * objectlist = objects.GetList();
   	objectlist_t::iterator iter;
   	for (iter = objectlist->begin(); iter != objectlist->end(); iter++) {
	  cEntity * object = *iter;
	  if (object->tileclass == TILE_CLASS_GROUND)  {
		cGroundObject * ground = (cGroundObject *) object;
		SDL_Rect rect;
		SDL_Surface * surface = 0;
	    if (ground->stretch) {
            i = ground->y;
            j = ground->x;
	        Texture * texture = pTextureBuffer->GetGroundTexmap(groundids[i][j]);
		    if (texture && texture->GetSurface()) {
               int alphas[4];
               alphas[0] = (int)alpha_values[i][j];
               alphas[1] = (int)alpha_values[i+1][j];
               alphas[2] = (int)alpha_values[i][j+1];
               alphas[3] = (int)alpha_values[i+1][j+1];
            
               int y2 = ground_draw_pos[i][j][1] - ground_draw_pos[i+1][j][1];
               int y3 = ground_draw_pos[i+1][j+1][1] - ground_draw_pos[i+1][j][1];
               int y4 = ground_draw_pos[i][j+1][1] - ground_draw_pos[i+1][j][1];
               int min_y;
            
			   rect.x = ground_draw_pos[i][j][0] + x;
			   rect.y = ground_draw_pos[i][j][1] + y;
   			   rect.w = 44;
			   rect.h = CalcStretchHeight(y2, y3, y4, min_y);
			   rect.y += -y2 + min_y;

			   bool isOK = true;
			   if (cliprect)
		          isOK &= !((rect.x > (cliprect->x + cliprect->w)) || (rect.y > (cliprect->y + cliprect->h)) || (cliprect->x > (rect.x + rect.w)) || (cliprect->y > (rect.y + rect.h)));
   		       if (isOK) {
			      SDL_Surface * surface = GetStretchedSurface(y2,y3,y4, alphas, texture->GetSurface(), rect.h, min_y);
		          if (!surface) {
			          Texture * texture = pTextureBuffer->GetGroundTexture(groundids[i][j]);
			          if (surface)
			              surface = texture->GetSurface();
                  }
                  if (surface) {
				      SDL_BlitSurface(surface, NULL, target, &rect);
			          SDL_FreeSurface(surface);
			      }   
               }
			} 
			else 
			   ground->stretch = false;
         } 
         if (!ground->stretch) {
			   Texture * texture = NULL;
               if (ground->tileid >= 0x4000)  
                              texture = pTextureBuffer->GetArtTexture(ground->tileid); 
               else
                              texture = pTextureBuffer->GetGroundTexture(ground->tileid);
	           if (texture)
		             surface = texture->GetSurface();
			   rect.x = ground->draw_x + x;
			   rect.y = ground->draw_y + y;
			   
			   if (surface) {
			   	   rect.w = surface->w;
			   	   rect.h = surface->h;
 			       SDL_BlitSurface(surface, NULL, target, &rect);  
			   }
			   
			}
     }
}
		
	return;
		
	
/*	int i, j;
		
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) 
			if (!custom_stretched[i][j]) {
			Texture * texture = pTextureBuffer->GetGroundTexture(groundids[i][j]);
			if (texture) {
				SDL_Surface * surface = texture->GetSurface();
				SDL_Rect rect;
				rect.x = ground_draw_pos[i][j][0] + x;
				rect.y = ground_draw_pos[i][j][1] + y;
				if (surface) {
					rect.w = surface->w;
					rect.h = surface->h;
									bool clip = false;
					if (cliprect)
						clip = (rect.x > (cliprect->x + cliprect->w)) || (rect.y > (cliprect->y + cliprect->h)) || (cliprect->x > (rect.x + rect.w)) || (cliprect->y > (rect.y + rect.h));
					if (!clip)
						SDL_BlitSurface(surface, NULL, target, &rect);
				}
			}
		}
		else {
				SDL_Rect rect;
				rect.x = ground_draw_pos[i][j][0] + x+ hotspots[i][j][0];
				rect.y = ground_draw_pos[i][j][1] + y + hotspots[i][j][1];
				SDL_Surface * surface = custom_stretched[i][j];
				rect.w = surface->w;
				rect.h = surface->h;
				bool clip = false;
			SDL_BlitSurface(surface, NULL, target, &rect);
		} */
}			

void cMapblock::RenderStatics(int x, int y, SDL_Surface * target, SDL_Rect * cliprect, int tile_type)
{
	if (!pTextureBuffer || !target)
		return;
	
		if (((outbox.x + outbox.w + x) < 0) || ((outbox.y + outbox.h + y) < 0) || (outbox.x + x > 1023) || (outbox.y + y> 1023))
			return; 
	
	int i, j;
		
	objectlist_t * objectlist = objects.GetList();
   	objectlist_t::iterator iter;
   	for (iter = objectlist->begin(); iter != objectlist->end(); iter++) {
		cEntity * object = *iter;
		if (object->tileclass & tile_type) {
		if (object->tileclass == TILE_CLASS_GROUND)  {
		  cGroundObject * ground = (cGroundObject *) object;
//printf("%i\n",		  ground->tileid);
				SDL_Rect rect;
				rect.x = ground->draw_x + x;
				rect.y = ground->draw_y + y;

            SDL_Surface * surface = NULL;//ground->surface;
            if (!surface) {
                        
               Texture * texture;
               if (ground->tileid >= 0x4000)  {
                              texture = pTextureBuffer->GetArtTexture(ground->tileid); 
               }
               else
                              texture = pTextureBuffer->GetGroundTexture(ground->tileid);
			   if (texture) 
			      surface = texture->GetSurface();
            } else {
                rect.x += hotspots[object->y][object->x][0];
                rect.y += hotspots[object->y][object->x][1];
            }
				if (surface) {
					rect.w = surface->w;
					rect.h = surface->h;
/*					bool clip = false;
					if (cliprect)
						clip = (rect.x > (cliprect->x + cliprect->w)) || (rect.y > (cliprect->y + cliprect->h)) || (cliprect->x > (rect.x + rect.w)) || (cliprect->y > (rect.y + rect.h));
					if (!clip) */
					object->hue = 200;
     					SDL_BlitSurface(surface, NULL, target, &rect);
				
			}
		}
		
		if (object->tileclass == TILE_CLASS_CHARACTER) {
		   cStaticCharacter * character = (cStaticCharacter *) object;
		   Uint32 base_anim = getAnimationBase(character->character->body());
		   cAnimation * anim = pAnimBuffer->GetAnimation(base_anim);
		   int frameid = character->character->anim_frame;
		   if (frameid >= anim->count())
		        frameid = 0;
		   sAnimationFrame * frame = anim->GetFrame(frameid);
		   if (frame && frame->surface) {
             SDL_Rect rect;
             rect.w = frame->surface->w;
             rect.h = frame->surface->h;
		     rect.x = x + object->draw_x- frame->centerx;
             rect.y = y + object->draw_y - rect.h- frame->centery;
  			SDL_BlitSurface(frame->surface, NULL, target, &rect);
           }
           for (int f=0; f < 30; f++) {
                      cCharacterEquip * equip = character->character->GetEquip(f);
                      if (equip) {
                               base_anim = getAnimationBase(equip->anim());
                      		   cAnimation * anim = pAnimBuffer->GetAnimation(base_anim);
                      		   if (anim) {
                   		   sAnimationFrame * frame = anim->GetFrame(frameid);
		   if (frame && frame->surface) {
             SDL_Rect rect;
             rect.w = frame->surface->w;
             rect.h = frame->surface->h;
		     rect.x = x + object->draw_x - frame->centerx;
             rect.y = y + object->draw_y - rect.h- frame->centery;
    			SDL_BlitSurface(frame->surface, NULL, target, &rect); 
           } 
           }}}
		   frameid ++;
           character->character->anim_frame = frameid;
		   //SDLscreen->DrawSurface(frame->surface, 100,100);
		}
		
		if (object->tileclass == TILE_CLASS_ITEM) {
		cStaticObject * item = (cStaticObject *) object;
/*		int tx = 7-object->y % 8;
		int ty = (object->x % 8);
		int z =( object->z + heightmap [tx][ty]) * 2; */
		SDL_Rect rect;
/*		rect.x = x + (tx+ty) * 22;
		rect.y = y +(ty-tx)*22 - z;*/
		rect.w = object->w;
		rect.h = object->h;
		rect.x = x + object->draw_x;
		rect.y = y + object->draw_y;
		bool clip = false;
		if (cliprect)
			clip = (rect.x > (cliprect->x + cliprect->w)) || (rect.y > (cliprect->y + cliprect->h)) || (cliprect->x > (rect.x + rect.w)) || (cliprect->y > (rect.y + rect.h));
		if (!clip) {
		Texture * texture = pTextureBuffer->GetArtTexture(item->tileid + 0x4000);
		if (texture) {
			SDL_Surface * surface = texture->GetSurface();
			if (surface) {
				if (!clip) {
					if (object->hue > 0) {
					     SDL_Surface * hued_surface = CreateHuedSurface(surface, object->hue - 1);
     					SDL_BlitSurface(hued_surface, NULL, target, &rect);
     					SDL_FreeSurface(hued_surface);
                    
					} else
     					SDL_BlitSurface(surface, NULL, target, &rect);
				}
			}
		}
	 
		}
		}
		}
	}
	

     
}

void cMapblock::AddDynamic (Uint32 id, int x, int y, int z, int tileid)
{
    cStaticObject * object = new cStaticObject;
	CreateObject(object, blockx * 8 + x, blocky * 8 + y, 
					z, tileid);
	object->id = id;
	objects.InsertDynamic(object);
}

void cMapblock::AddCharacter (Uint32 id, int x, int y, int z, cCharacter * c_char)
{
    cStaticCharacter * character = new cStaticCharacter;
    character->x = x;
    character->y = y;
    character->z = z;
    character->blockx = blockx;
    character->blocky = blocky;
    character->character = c_char;
	character->id = id;
	int tx = 7-character->y % 8;
	int ty = (character->x % 8);
	character->draw_x = (tx+ty) * 22;
	character->draw_y = (ty-tx) * 22 - z * 4;
	objects.InsertCharacter(character);
}

void cMapblock::DeleteDynamic (Uint32 id)
{
	objects.DeleteDynamic(id);
}


int cMapblock::GetGroundZ(Uint32 x, Uint32 y)
{
	if ((x < 8) && (y < 8))
		return heightmap[y][x];
	return 0;
}



int cMapblock::GetWalkZ(Uint32 dstx, Uint32 dsty, int srcz)
{
	if ((dstx >= 8) || (dsty >= 8))
		return 255;
	int ground = heightmap[dsty][dstx];
     	
	objectlist_t * objectlist = objects.GetList();
   	objectlist_t::iterator iter;
   	for (iter = objectlist->begin(); iter != objectlist->end(); iter++) {
		cEntity * object = *iter;
		if (object->tileclass != TILE_CLASS_ITEM)
		  continue;
        cStaticObject * item = (cStaticObject *) object;
		if ((object->x == dstx) && (object->y == dsty)) {
			if (item->tiledata_flags & (TILEDATAFLAG_SURFACE | TILEDATAFLAG_BRIDGE | TILEDATAFLAG_STAIRBACK | TILEDATAFLAG_STAIRRIGHT)) 
			{
				int z = item->z + item->height;
				if ((z <= srcz + 5) && (z > ground))
					ground = z;
			}
			if (item->tiledata_flags & (TILEDATAFLAG_WALL | TILEDATAFLAG_IMPASSABLE)) 
			{
				int z = object->z + object->height;
				if (!((srcz + 5 < object->z) || (srcz >= z))) 
					return 255;
			}
		}
	}
	return ground;
}


void cMapblock::GetBounds(INT_Rect * rect, int x, int y)
{
    if ((int) outbox.x + x < rect->x) rect->x = (int) outbox.x + x;
    if ((int) outbox.y + y < rect->y) rect->y = (int) outbox.y + y;
    if ((int) outbox.x + x + (int) outbox.w > rect->x + rect->w) 
        rect->w = (int) outbox.x + x + (int) outbox.w - rect->x;
    if ((int) outbox.y + y + (int) outbox.h > rect->y + rect->h) 
        rect->h = (int) outbox.y + y + (int) outbox.h - rect->y;
}

