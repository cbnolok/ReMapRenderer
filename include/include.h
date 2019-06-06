//
// File: include.h
// Created by: Alexander Oster - tensor@ultima-iris.de
//
/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/

#ifndef _INCLUDE_H_
#define _INCLUDE_H_

extern int texture_mem;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define SDL_RMASK 0xff000000
    #define SDL_GMASK 0x00ff0000
    #define SDL_BMASK 0x0000ff00
    #define SDL_AMASK 0x000000ff
#else
    #define SDL_AMASK 0xff000000
    #define SDL_BMASK 0x00ff0000
    #define SDL_GMASK 0x0000ff00
    #define SDL_RMASK 0x000000ff
#endif

#define SDL_BITMASK SDL_RMASK, SDL_GMASK, SDL_BMASK, SDL_AMASK
#define SDL_RGB_BITMASK SDL_RMASK, SDL_GMASK, SDL_BMASK, 0

#define SDL_RGB_16_BITMASK 0x7C00, 0x3E0, 0x1F, 0
#define SDL_RGBA_16_BITMASK 0x7C00, 0x3E0, 0x1F, 0x8000

#endif //_INCLUDE_H_
