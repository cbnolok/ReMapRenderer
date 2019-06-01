//
// File: Renderer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Renderer.h"
#include "renderer/SDLScreen.h"

#include "include.h"
#include "Config.h"
#include "Debug.h"

#include <iostream>

#include "renderer/TextureBuffer.h"
#include "renderer/MapBuffer.h"

using namespace std;

extern SDLScreen *SDLscreen;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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

            if (!((dx > RENDER_CACHE_WIDTH * 2) || (dx < -RENDER_CACHE_WIDTH * 2) || (dy > RENDER_CACHE_HEIGHT * 2) || (dy < -RENDER_CACHE_HEIGHT * 2))) {

                cMapblock * block = pMapbuffer->CreateBlock(x, y);

                if (block)
                    block->RenderGround(dx, dy, SDLscreen->screen, NULL);
            }
        }
}

void Renderer::RenderStatics(SDL_Surface * target, int mx, int my)
{
    for (int x = nConfig::minblockx; x <= nConfig::maxblockx; ++x)
        for (int y = nConfig::minblocky; y <= nConfig::maxblocky; ++y)
        {
            int tx = x - nConfig::minblockx;
            int ty = nConfig::maxblocky - y;

            int dx = (tx+ty)*176 + hotspotx + mx;
            int dy = (tx-ty)*176 + hotspoty + my;

            if (!((dx > RENDER_CACHE_WIDTH * 2) || (dx < -RENDER_CACHE_WIDTH * 2) || (dy > RENDER_CACHE_HEIGHT * 2) || (dy < -RENDER_CACHE_HEIGHT * 2))) {

                cMapblock * block = pMapbuffer->CreateBlock(x, y);


                if (block)
                    block->RenderStatics(dx, dy, SDLscreen->screen, NULL, TILE_CLASS_ITEM);
            }
        }
}

void Renderer::Rebuild (int mx, int my)
{
    RenderTerrain(NULL, mx, my);
    RenderStatics(NULL, mx, my);
}

INT_Rect Renderer::CalcRenderBox()
{
    INT_Rect outbox ={};

    printf("Calculating render size in pixels (non-zoomed)...");
    for (int x = nConfig::minblockx; x <= nConfig::maxblockx; x++)
        for (int y = nConfig::minblocky; y <= nConfig::maxblocky; y++)
        {
            cMapblock * block = pMapbuffer->CreateBlock(x, y);
            int tx = x - nConfig::minblockx;
            int ty = nConfig::maxblocky - y;

            if (block) {
                block->GetBounds(&outbox, (tx+ty)*176, (tx-ty)*176);
            }
        }
    hotspotx = - outbox.x;
    hotspoty = - outbox.y;
    printf(" %i x %i\n", outbox.w, outbox.h);

    return outbox;
}
