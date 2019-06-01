//
// File: StaticObjects.h
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

#ifndef _STATICOBJECTS_H_
#define _STATICOBJECTS_H_

#include <vector>


#define TILE_CLASS_GROUND    1
#define TILE_CLASS_ITEM      2
#define TILE_CLASS_CHARACTER 4

#define TILE_CLASS_ALL       15

struct TDStaticFlag
{
    enum TDS_FLAGS : unsigned long long
    {
        None = 0x00000000,
        Background = 0x00000001,
        Weapon = 0x00000002,
        Transparent = 0x00000004,
        Translucent = 0x00000008,
        Wall = 0x00000010,
        Damaging = 0x00000020,
        Impassable = 0x00000040,
        Wet = 0x00000080,
        Unknown1 = 0x00000100,
        Surface = 0x00000200,
        Bridge = 0x00000400,
        Generic = 0x00000800,
        Window = 0x00001000,
        NoShoot = 0x00002000,
        ArticleA = 0x00004000,
        ArticleAn = 0x00008000,
        Internal = 0x00010000,
        Foliage = 0x00020000,
        PartialHue = 0x00040000,
        Unknown2 = 0x00080000,
        Map = 0x00100000,
        Container = 0x00200000,
        Wearable = 0x00400000,
        LightSource = 0x00800000,
        Animation = 0x01000000,
        NoDiagonal = 0x02000000,
        Unknown3 = 0x04000000,
        Armor = 0x08000000,
        Roof = 0x10000000,
        Door = 0x20000000,
        StairBack = 0x40000000,
        StairRight = 0x80000000
    };
};

class cEntity {
public:
    int tileclass;
    int x, y, z;
    int blockx, blocky;
    int w, h;
    int draw_x, draw_y;
    int tileid;
    int hue;
    unsigned int flags;
    unsigned long long tiledata_flags;
    unsigned int id;
    int height;
    virtual ~cEntity () { };
};

class cStaticObject: public cEntity
{
public:
    cStaticObject () {
        id = 0;
    }
    virtual ~cStaticObject () { };
};

class cGroundObject: public cEntity
{
public:
    int stretch;
    cGroundObject ();
    virtual ~cGroundObject ();
};


typedef std::vector<class cEntity *>  objectlist_t;

class StaticObjectList
{
private:
    objectlist_t objectlist;

public:
    StaticObjectList ()
    { }

    ~StaticObjectList ()
    {
        Clear();
    }

    void Clear (void);

    cStaticObject * AddStatic ();
    cGroundObject * AddGround ();
    /*
    void InsertDynamic (cStaticObject * object);
    void DeleteDynamic (Uint32 id);
    */

    objectlist_t * GetList(void) { return &objectlist; }
    //int GetCount(void) const     { return objectlist.size(); }

    void Sort();
};


#endif //_STATICOBJECTS_H_
