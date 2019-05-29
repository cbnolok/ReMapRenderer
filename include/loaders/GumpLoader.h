//
// File: GumpLoader.h
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

#ifndef _GUMPLOADER_H_
#define _GUMPLOADER_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include "include.h"
#include "../renderer/Texture.h"


class cGumpLoader
{
private:
	std::ifstream * gumpfile;
	std::ifstream * gumpindex;
	unsigned int gump_count;
 
public:
    cGumpLoader (std::string filename, std::string indexname);
   ~cGumpLoader ();
   
   Texture * LoadGump(int index);
   bool CheckGump(int index);

protected:

};

extern cGumpLoader * pGumpLoader;

#endif //_GROUNDTEXTURES_H_
