//
// File: Renderer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <cstdio>

#include "renderer/Renderer.h"
#include "renderer/SDLScreen.h"
#include "renderer/TextureBuffer.h"
#include "renderer/MapBuffer.h"

#include "include.h"
#include "Config.h"
#include "Debug.h"

using namespace std;

extern SDLScreen *SDLscreen;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


void Renderer::Rebuild (int mx, int my)
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
                    block->RenderType(TILE_CLASS_GROUND, dx, dy, SDLscreen->screen, NULL);
            }
        }
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
                    block->RenderType(TILE_CLASS_ITEM, dx, dy, SDLscreen->screen, NULL);
            }
        }
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
