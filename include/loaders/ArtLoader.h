//
// File: ArtLoader.h
// Created by:  Alexander Oster - tensor@ultima-iris.de
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

#ifndef _ARTLOADER_H_
#define _ARTLOADER_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include "include.h"
#include "../renderer/Texture.h"


class cArtLoader
{
private:
	std::ifstream * artfile;
	std::ifstream * artindex;
   
   Texture * LoadGroundArt(int index);
   Texture * LoadStaticArt(int index);
 
public:
    cArtLoader (std::string filename, std::string indexname);
   ~cArtLoader ();
   
   Texture * LoadArt(int index);

protected:
	unsigned int art_count;
};

extern cArtLoader * pArtLoader;

#endif //_GROUNDTEXTURES_H_
