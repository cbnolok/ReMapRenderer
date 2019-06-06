//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//

#include <iostream>
#include "SDL/SDL.h"
#include "renderer/SDLScreen.h"
#include "include.h"


int texture_mem = 0;

float BilinearTable[32][32][4];

static void CalcBilinearTable ()
{
    float fracx, fracy;
    for (int y = 0; y < 32; y++)
        for (int x = 0; x < 32; x++)
        {
            fracx = (float)x / 32.0f;
            fracy = (float)y / 32.0f;
            BilinearTable[y][x][0] = fracx * fracy;
            BilinearTable[y][x][1] = (1.0f - fracx) * fracy;
            BilinearTable[y][x][2] = fracx * (1.0f - fracy);
            BilinearTable[y][x][3] = (1.0f - fracx) * (1.0f - fracy);
        }
}

SDLScreen::SDLScreen()
{
    screen = NULL;
    CalcBilinearTable();
}

SDLScreen::~SDLScreen()
{
    if (screen)
        SDL_FreeSurface(screen);
    SDL_Quit();
}

int SDLScreen::Init(int width, int height)
{
    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* get a SDL surface */
    screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, SDL_RGB_BITMASK);
    //  	screen = SDL_CreateRGBSurface(width, height, 16, videoFlags);
        /* Verify there is a surface */
    if (!screen) {
        cerr << "Video mode set failed: " << SDL_GetError() << endl;
        exit(1);
    }

    return true;
}

