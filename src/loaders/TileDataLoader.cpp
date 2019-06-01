//
// File: TileDataLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <cstring>
#include <iostream>
#include "loaders/TileDataLoader.h"
#include "Debug.h"

using namespace std;

cTileDataLoader * pTileDataLoader = NULL;
int tiledataMaxID = 0;

cTileDataLoader::cTileDataLoader (std::string filename)
{
    highseas = false;
    tiledatafile = new ifstream(filename.c_str(), ios::ate | ios::binary);
    if (!tiledatafile) {
        pDebug.Log("Out of Memory in ArtLoader::ArtLoader", __FILE__, __LINE__,
            LEVEL_ERROR);
        return;
    }

    if (!tiledatafile->is_open()) {
        /*
        string errstr;
        errstr = "Could not load ground tiledata file: ";
        errstr += filename;
        pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
        */
        delete tiledatafile;
        tiledatafile = NULL;
        return;
    }

    streamoff filesize = tiledatafile->tellg();
    tiledatafile->seekg(0, ios::beg);
    if (filesize >= 3188736)
        highseas = true;

    tiledataMaxID = highseas ? 0x7FFFF : 0xC0000;
}

cTileDataLoader::~cTileDataLoader ()
{
    delete tiledatafile;
}

bool cTileDataLoader::LoadEntry (unsigned int index, struct TileDataStaticEntry * entry)
{
    if ((index >= tiledataMaxID) || !entry || !tiledatafile)
        return false;

    static constexpr unsigned entries_per_group = 32;

    static const unsigned terrain_entrysize = highseas ? 30 : 26;
    static const unsigned terrain_groupsize = 4 + (entries_per_group * terrain_entrysize);
    static const unsigned terrain_sectionsize = ((0x4000 / entries_per_group) * terrain_groupsize) + ((0x4000 % entries_per_group) * terrain_entrysize);

    static const unsigned static_entrysize = highseas ? sizeof(TileDataStaticEntry) : sizeof(TileDataStaticEntry_PreHS); // 41 : 37
    static const unsigned static_groupsize = 4 + (entries_per_group *  static_entrysize);

    unsigned int group_idx    = index / entries_per_group;
    unsigned int idx_in_group = index % entries_per_group;

    // 512 * 836 is the size of the landtile data, skip it, we want statics data.
    // a static tile group is 1188 bytes
    tiledatafile->seekg(terrain_sectionsize + (static_groupsize * group_idx) + 4 + (idx_in_group * static_entrysize), ios::beg);

    if (highseas)
    {
        tiledatafile->read((char *)entry, static_entrysize);
    }
    else
    {
        *entry ={};
        struct TileDataStaticEntry_PreHS oldentry;
        tiledatafile->read((char *)&oldentry, static_entrysize);
        entry->flags = oldentry.flags;
        entry->weight = oldentry.weight;
        entry->quality = oldentry.quality;
        entry->unknown = oldentry.unknown;
        entry->unknown1 = oldentry.unknown1;
        entry->quantity = oldentry.quantity;
        entry->animid = oldentry.animid;
        entry->hue = oldentry.hue;
        entry->lightidx = oldentry.lightidx;
        entry->height = oldentry.height;
        strncpy(entry->name, oldentry.name, 19);
        entry->name[19] = '\0';
    }

    return true;
}
