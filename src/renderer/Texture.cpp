//
// File: Texture.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "SDL/SDL.h"
#include "include.h"
#include "renderer/Texture.h"
#include "Debug.h"

using namespace std;

Texture::Texture (unsigned int id): _id(id)
{
    _surface = NULL;
}

Texture::~Texture ()
{
    if (_surface) {
        texture_mem -= _surface->pitch *_surface->h;
        SDL_FreeSurface(_surface);
    }
}

int Texture::LoadFromData(void * data, int width, int height, int bits_per_pixel)
{
    if (_surface)
        SDL_FreeSurface(_surface);

    _surface = NULL;

    if (bits_per_pixel != 32)
        return false;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
         on the endianness (byte order) of the machine */

         //  surface = SDL_CreateRGBSurface&*From(data, width, height, bits_per_pixel, pitch, rmask, gmask, bmask, amask);
    _surface = SDL_CreateRGBSurface(0, width, height, bits_per_pixel, SDL_BITMASK);
    texture_mem += _surface->w * _surface->h * 4;

    if (!_surface) {
        pDebug.Log("Out of Memory in Texture::LoadFromData ()", __FILE__, __LINE__, LEVEL_ERROR);
        return false;
    }
    memcpy (_surface->pixels, data, width* height * 4);


    return (true);
}

int Texture::Create16(int width, int height)
{
    if (_surface)
        SDL_FreeSurface(_surface);

    _surface = NULL;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */

       //  surface = SDL_CreateRGBSurface&*From(data, width, height, bits_per_pixel, pitch, rmask, gmask, bmask, amask);
    _surface = SDL_CreateRGBSurface(0, width, height, 16, SDL_RGB_16_BITMASK);
    SDL_SetColorKey(_surface, SDL_TRUE, 0);

    if (!_surface) {
        pDebug.Log("Out of Memory in Texture::LoadFromData ()", __FILE__, __LINE__, LEVEL_ERROR);
        return false;
    }

    texture_mem += _surface->pitch * _surface->h;
    return (true);
}

int Texture::LoadFromFile(const char * filename)
{
    if (_surface)
        SDL_FreeSurface(_surface);

    _surface = NULL;//IMG_Load( filename );

    if (!_surface)
    {
        char errorStr[512];
        sprintf(errorStr, "Error loading texture: %s", filename);
        pDebug.Log(errorStr, __FILE__, __LINE__, LEVEL_ERROR);
        return false;
    }

    return true;
}

int Texture::GetHeight(void)
{
    if (!_surface)
        return 0;
    return _surface->h;
}

int Texture::GetWidth(void)
{
    if (!_surface)
        return 0;
    return _surface->w;
}

