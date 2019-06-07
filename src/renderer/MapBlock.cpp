//
// File: MapBlock.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <cmath>
#include "renderer/MapBlock.h"
#include "Debug.h"
#include "include.h"
#include "Config.h"
#include "uotype.h"
#include "loaders/Map.h"
#include "renderer/TextureBuffer.h"
#include "renderer/StaticObjects.h"
#include "renderer/SDLScreen.h"
#include "loaders/TileDataLoader.h"
#include "loaders/HueLoader.h"

using namespace std;


static SDL_Surface * CreateHuedSurface(SDL_Surface * surface, Uint16 HueID)
{
    if (!surface)
        return NULL;

    SDL_Surface * result = SDL_CreateRGBSurface(0, surface->w, surface->h, 16, SDL_RGB_16_BITMASK);
    SDL_SetColorKey(result, SDL_TRUE, 0);

    Uint16 * src = (Uint16*)surface->pixels;
    Uint16 * dst = (Uint16*)result->pixels;
    int size = surface->w * surface->h;

    const struct stHue * hue = pHueLoader->getHue(HueID);
    for (int i = 0; i < size; ++i) {
        if (*src)
            *dst = hue->colors[(*src >> 10) & 31];
        ++src; ++dst;
    }
    return result;
}

static inline void LightenPixel(Uint32 * pixel, short value)
{
    Uint8 * p = (Uint8 *)pixel;
    for (unsigned char i = 0; i < 3; ++i) {
        short v = *p + value;
        if (v < 0)   v = 0;
        else if (v > 255) v = 255;
        *p = (Uint8)v;
        ++p;
    }
}

static inline int CalcStretchHeight(int y2, int y3, int y4, int & min_y)
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

static SDL_Surface * GetStretchedSurface (int y2, int y3, int y4, int * alphas, SDL_Surface * texture, int h, int min_y)
{
    if (!texture)
        return NULL;

    const int tsize = texture->w - 1;
    const float tfac = (float)texture->w / 22.0f;

    //y2 = 0; y3 = 0; y4 = 0;

    const int up_point = y2;
    const int down_point = y3;
    const int right_point = y4;

    if (up_point > down_point)
        return NULL;

    if (!h)
        return NULL;

    //hotspot[0] = 0;
    //hotspot[1] = -y2 + min_y;// - 22;

    SDL_Surface * surface = SDL_CreateRGBSurface(0, 44, h, 32, SDL_BITMASK);
    float m1 = (float)up_point / 22.0f;
    float m2 = (float)down_point / 22.0f;

    float yp1 = 0.0f;
    float yp2 = 0.0f;

    Uint32 * qtex = (Uint32 *)texture->pixels;
    Uint32 * qsurf = (Uint32 *)surface->pixels;

    for (int x = 0; x < 22; ++x)
    {
        float tx = 0.0f;
        float ty = x * tfac;

        Uint32 * p = qsurf + x + ((int)yp1 - min_y) * 44;
        float dy = yp2 - yp1 + 1;
        if (!dy)
            dy = 0.0001f;
        float m = ty / dy;

        for (int y = (int)yp1; y <= yp2; ++y)
        {
            Uint32 *src = qtex + (int)tx + (tsize - (int)ty) * texture->w;
            *p = *src;
            //      *p = 0xff8080ff;

            if (nConfig::render_shadows)
            {
                float * table = BilinearTable[(tsize - (int)tx) * 32 / texture->w][((int)ty) * 32 / texture->w];
                //        float * table = BilinearTable[(tsize - (int) ty) * 32 / texture->w][(int) tx * 32 / texture->w];
                float alpha = table[0] * alphas[0] + table[1] * alphas[1] + table[2] * alphas[2] + table[3] * alphas[3];
                LightenPixel(p, (short)alpha);
            }

            p += 44;
            tx += m;
            ty -= m;
        }
        yp1 += m1;
        yp2 += m2;
    }
    m1 = (float)(up_point - right_point) / 22.0f;
    m2 = (float)(down_point - right_point) / 22.0f;
    for (short x = 22; x < 44; ++x)
    {
        float tx = (x - 22) * tfac;
        float ty = (float)(tsize - 1);

        const float dy = yp2 - yp1 + 1.0f;
        float m = (ty - tx) / dy;

        Uint32 * p = qsurf + x + ((int)yp1 - min_y) * 44;
        for (int y = (int)yp1; y <= yp2; ++y)
        {
            Uint32 *src = qtex + (int)tx + (tsize - (int)ty) * texture->w;
            *p = *src;
            //        *p = 0xff8080ff;

            if (nConfig::render_shadows)
            {
                float * table = BilinearTable[(tsize - (int)tx) * 32 / texture->w][((int)ty) * 32 / texture->w];
                float alpha = table[0] * alphas[0] + table[1] * alphas[1] + table[2] * alphas[2] + table[3] * alphas[3];
                //        float alpha = table[0] * alphas[3] + table[1] * alphas[2] + table[2] * alphas[1] + table[3] * alphas[0];
                LightenPixel(p, (short)alpha);
            }            

            p += 44;
            tx += m;
            ty -= m;
        }
        yp1 -= m1;
        yp2 -= m2;
    }
    return surface;
}


cMapblock::cMapblock()
{
    blockx = 0;
    blocky = 0;
    memset(heightmap, 0, sizeof heightmap);
    memset(groundids, 0, sizeof groundids);
    unsigned char i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            custom_stretched[i][j] = NULL;
            ground_draw_pos[i][j][0] = 0;
            ground_draw_pos[i][j][1] = 0;
        }
}

cMapblock::~cMapblock()
{
    unsigned char i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            if (custom_stretched[i][j])
                SDL_FreeSurface(custom_stretched[i][j]);
}


static void CreateObject(cEntity *object, unsigned int x, unsigned int y,
    int z, unsigned int tileid, bool skiptiledata = false)
{
    object->x = x % 8;
    object->y = y % 8;
    object->z = z;
    object->tileid = tileid;
    object->hue = 0;
    if (!skiptiledata && pTileDataLoader)
    {
        struct TileDataStaticEntry entry;
        pTileDataLoader->LoadEntry(tileid, &entry);
        object->height = entry.height;
        object->tiledata_flags = entry.flags;
    }
    if (pTextureBuffer)
    {
        Texture * texture = pTextureBuffer->GetArtTexture(tileid + TILEDATA_MAX_ID_LAND);
        if (texture)
        {
            object->w = texture->GetWidth();
            object->h = texture->GetHeight();
        }
    }
}

static inline void ExtractCell (struct MulCell * cell, Sint8 * heightmap, Uint16 * groundid)
{
    if (heightmap)
        *heightmap = cell->z;
    if (groundid)
        *groundid = cell->tileid;
}

bool cMapblock::Generate(int blockx, int blocky)
{
    if (!pMapLoader || !pTextureBuffer)
        return false;
    struct MulBlock block;

    this->blockx = blockx;
    this->blocky = blocky;


    unsigned char xc, yc;

    pMapLoader->LoadMapBlock(blockx, blocky, &block);
    for (yc = 0; yc < 8; ++yc)
        for (xc = 0; xc < 8; ++xc)
            ExtractCell(&block.cells[yc][xc], &heightmap[yc][xc], &groundids[yc][xc]);

    pMapLoader->LoadMapBlock(blockx + 1, blocky, &block);
    for (yc = 0; yc < 8; ++yc)
        for (xc = 0; xc < 2; ++xc)
            ExtractCell(&block.cells[yc][xc], &heightmap[yc][8 + xc], NULL);

    pMapLoader->LoadMapBlock(blockx, blocky + 1, &block);
    for (yc = 0; yc < 2; ++yc)
        for (xc = 0; xc < 8; ++xc)
            ExtractCell(&block.cells[yc][xc], &heightmap[8+yc][xc], NULL);

    pMapLoader->LoadMapBlock(blockx + 1, blocky + 1, &block);
    for (yc = 0; yc < 2; ++yc)
        for (xc = 0; xc < 2; ++xc)
            ExtractCell(&block.cells[yc][xc], &heightmap[8+yc][8+xc], NULL);

    int min_x = 1000000;
    int max_x = -1000000;
    int min_y = 1000000;
    int max_y = -1000000;

    int staticcount = 0;
    struct staticinfo * statics = pMapLoader->LoadStatics(blockx, blocky, staticcount);
    struct staticinfo * statics_p = statics;
    int x = blockx * 8, y = blocky * 8;
    //std::map<unsigned, bool> staticsLand;

    for (int istatic = 0; istatic < staticcount; ++istatic)
    {
        /*
        cEntity tmp;
        cEntity * object = NULL;
        CreateObject(&tmp, x + statics_p->x, y + statics_p->y, statics_p->z, statics_p->TileID);
        if (tmp.tiledata_flags & TILEDATAFLAG_WET)
        {
        // Water statics
        object = objects.AddGround();
        *object = tmp;
        object->tileclass = TILE_CLASS_GROUND;
        object->tileid = statics_p->TileID + TILEDATA_MAX_ID_LAND;
        static_cast<cGroundObject *>(object)->stretch = 0;
        }
        else
        {
        object = objects.AddStatic();
        *object = tmp;
        object->hue = statics_p->Hue;
        }*/

        cEntity * object;
        if ((statics_p->TileID >= 6038) && (statics_p->TileID <= 6066))
        {
            // Water statics
            object = objects.AddGround();
            CreateObject(object, x + statics_p->x, y + statics_p->y, statics_p->z, statics_p->TileID, true);
            object->tileclass = TILE_CLASS_GROUND;
            object->tileid = statics_p->TileID + TILEDATA_MAX_ID_LAND;
            cGroundObject * gObj = static_cast<cGroundObject*>(object);
            gObj->stretch = 0; // The land map appears different probably because of stretch = 1, that causes the call to GetStretchedSurface and LightenPixel
        }
        else
        {
            object = objects.AddStatic();
            CreateObject(object, x + statics_p->x, y + statics_p->y, statics_p->z, statics_p->TileID);
            object->hue = statics_p->Hue;
        }

        const int tx = object->y;
        const int ty = object->x;
        /*
        if (object->tileclass == TILE_CLASS_GROUND)
        {
            staticsLand[unsigned((tx << 16)|ty)] = true;
            heightmap[tx][ty] = object->z;
        }
        */

        const int z = (object->z) * 4;
        object->draw_x = ((7-tx)+ty) * 22;
        object->draw_y = (ty-(7-tx)) * 22 - z - object->h;
        if (object->draw_x < min_x)
            min_x = object->draw_x;
        if (object->draw_y < min_y)
            min_y = object->draw_y;
        if (object->draw_x + object->w > max_x)
            max_x = object->draw_x + object->w;
        if (object->draw_y + object->h > max_y)
            max_y = object->draw_y + object->h;

        ++statics_p;
    }
    free(statics);

    for (xc = 0; xc < 10; ++xc)
        for (yc = 0; yc < 10; ++yc)
        {
            const int z = ((Sint32)heightmap[xc][yc]) *4;
            const int d_x = ((7-xc)+yc) *22;
            const int d_y = (yc-(7-xc)) *22 - z - 44;
            ground_draw_pos[xc][yc][0] = d_x;
            ground_draw_pos[xc][yc][1] = d_y;
        }

    for (xc = 0; xc < 9; ++xc)
        for (yc = 0; yc < 9; ++yc)
        {
            const int y2 = ground_draw_pos[xc][yc][1] - ground_draw_pos[xc+1][yc][1];
            const int y3 = ground_draw_pos[xc+1][yc+1][1] - ground_draw_pos[xc+1][yc][1];
            alpha_values[xc][yc] = 30000.0f / (float)(y2 * y2 + y3 * y3 + 1) - 60.0f;
            if (alpha_values[xc][yc] > 10.0f) alpha_values[xc][yc] = 10.0f;
            if (alpha_values[xc][yc] < -50.0f) alpha_values[xc][yc] = -50.0f;
        }

    for (xc = 0; xc < 8; ++xc)
        for (yc = 0; yc < 8; ++yc)
        {
            //if (staticsLand.find(unsigned((xc << 16)|yc)) != staticsLand.end())
            //    continue;

            cGroundObject * object = objects.AddGround();
            object->x = yc;
            object->y = xc;
            object->z = heightmap[xc][yc];
            object->tileid = groundids[xc][yc];
            object->draw_x = ground_draw_pos[xc][yc][0];
            object->draw_y = ground_draw_pos[xc][yc][1];
            object->stretch = 1;
            object->hue = 0;

            static constexpr int landtile_w = 44;
            static constexpr int landtile_h = 44;
            if (object->draw_x < min_x)
                min_x = object->draw_x;
            if (object->draw_y < min_y)
                min_y = object->draw_y;
            if (object->draw_x + landtile_w > max_x)
                max_x = object->draw_x + landtile_w;
            if (object->draw_y + landtile_h > max_y)
                max_y = object->draw_y + landtile_h;

        }

    outbox.x = min_x;
    outbox.y = min_y;
    outbox.w = max_x - min_x;
    outbox.h = max_y - min_y;

    objects.Sort();

    return true;
}


void cMapblock::RenderType(int type, int x, int y, SDL_Surface * target, SDL_Rect * cliprect)
{
    if (!pTextureBuffer || !target)
        return;
    if (((outbox.x + outbox.w + x) < 0) || ((outbox.y + outbox.h + y) < 0) || (outbox.x + x >= RENDER_CACHE_WIDTH) || (outbox.y + y >= RENDER_CACHE_HEIGHT))
        return;

    objectlist_t * objectlist = objects.GetList();
    for (cEntity * object : *objectlist)
    {
        if ((type & TILE_CLASS_GROUND) && (object->tileclass == TILE_CLASS_GROUND))
            RenderObjectGround(object, x, y, target, cliprect);
        else if ((type & TILE_CLASS_ITEM) && (object->tileclass == TILE_CLASS_ITEM))
            RenderObjectItem(object, x, y, target, cliprect);
    }

    return;
}

void cMapblock::RenderObjectGround(cEntity *object, int x, int y, SDL_Surface * target, SDL_Rect * cliprect)
{
    cGroundObject * ground = static_cast<cGroundObject *>(object);
    SDL_Rect rect;
    if (ground->stretch)
    {
        int i = ground->y;
        int j = ground->x;
        Texture * texture = pTextureBuffer->GetGroundTexmap(groundids[i][j]);
        if (texture && texture->GetSurface())
        {
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
            if (isOK)
            {
                SDL_Surface * surface = GetStretchedSurface(y2, y3, y4, alphas, texture->GetSurface(), rect.h, min_y);
                if (!surface)
                {
                    Texture * texture = pTextureBuffer->GetGroundTexture(groundids[i][j]);
                    if (surface)
                        surface = texture->GetSurface();
                }
                if (surface)
                {
                    SDL_BlitSurface(surface, NULL, target, &rect);
                    SDL_FreeSurface(surface);
                }
            }
        }
        else
            ground->stretch = false;
    }
    if (!ground->stretch)
    {
        Texture * texture = NULL;
        if (ground->tileid >= TILEDATA_MAX_ID_LAND)
            texture = pTextureBuffer->GetArtTexture(ground->tileid);
        else
            texture = pTextureBuffer->GetGroundTexture(ground->tileid);

        SDL_Surface * surface = NULL;
        if (texture)
            surface = texture->GetSurface();

        rect.x = ground->draw_x + x;
        rect.y = ground->draw_y + y;
        if (surface)
        {
            rect.w = surface->w;
            rect.h = surface->h;
            SDL_BlitSurface(surface, NULL, target, &rect);
        }
    }
}

void cMapblock::RenderObjectItem(cEntity *object, int x, int y, SDL_Surface * target, SDL_Rect * cliprect)
{
    if (!nConfig::render_underground)
    {
        int tx = object->y % 8;
        int ty = (object->x % 8);
        //int z =( object->z + heightmap [tx][ty]) * 2; 
        int zLand = heightmap [tx][ty];
        int zObj = object->z + object->height; 
        if (zObj <= zLand)
            return;
    }
        
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
    if (!clip)
    {
        cStaticObject * item = static_cast<cStaticObject *>(object);
        Texture * texture = pTextureBuffer->GetArtTexture(item->tileid + TILEDATA_MAX_ID_LAND);
        if (texture)
        {
            SDL_Surface * surface = texture->GetSurface();
            if (surface)
            {
                if (object->hue > 0)
                {
                    SDL_Surface * hued_surface = CreateHuedSurface(surface, object->hue - 1);
                    SDL_BlitSurface(hued_surface, NULL, target, &rect);
                    SDL_FreeSurface(hued_surface);

                }
                else
                    SDL_BlitSurface(surface, NULL, target, &rect);
            }
        }

    }
}

void cMapblock::GetBounds(INT_Rect * rect, int x, int y) const
{
    if ((int)outbox.x + x < rect->x)
        rect->x = (int)outbox.x + x;
    if ((int)outbox.y + y < rect->y)
        rect->y = (int)outbox.y + y;
    if ((int)outbox.x + x + (int)outbox.w > rect->x + rect->w)
        rect->w = (int)outbox.x + x + (int)outbox.w - rect->x;
    if ((int)outbox.y + y + (int)outbox.h > rect->y + rect->h)
        rect->h = (int)outbox.y + y + (int)outbox.h - rect->y;
}
