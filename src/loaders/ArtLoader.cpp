//
// File: ArtLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "SDL/SDL.h"
#include "loaders/ArtLoader.h"
#include "loaders/VerdataLoader.h"
#include "Debug.h"
#include "uotype.h"

using namespace std;


cArtLoader * pArtLoader = NULL;

cArtLoader::cArtLoader (std::string filename, std::string indexname)
{
    string errstr;
    art_count = 0;

    artfile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if (!artfile) {
        pDebug.Log("Out of Memory in ArtLoader::ArtLoader", __FILE__, __LINE__,
            LEVEL_ERROR);
        return;
    }

    artindex = new ifstream(indexname.c_str(), ios::in | ios::binary);
    if (!artindex) {
        delete artfile;
        artfile = NULL;
        pDebug.Log("Out of Memory in ArtLoader::ArtLoader", __FILE__, __LINE__,
            LEVEL_ERROR);
        return;
    }

    if (!artfile->is_open()) {
        errstr = "Could not load ground texture file: ";
        errstr += filename;
        pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
        delete artfile;
        delete artindex;
        artfile = NULL;
        artindex = NULL;
        return;
    }

    if (!artindex->is_open()) {
        errstr += indexname;
        pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
        delete artfile;
        delete artindex;
        artfile = NULL;
        artindex = NULL;
        return;
    }

    artindex->seekg(0, ios::end);
    size_t idxE = artindex->tellg();
    art_count = (unsigned int)(idxE / 12);
}

cArtLoader::~cArtLoader ()
{
    delete artfile;
    delete artindex;
}

Texture * cArtLoader::LoadArt(int index)
{
    if ((index < 0) || ((unsigned int)index >= art_count))
        return NULL;

    if (index < TILEDATA_MAX_ID_LAND)
        return LoadGroundArt(index);

    return LoadStaticArt(index);
}


Texture * cArtLoader::LoadGroundArt(int index)
{
    if ((index < 0) || ((unsigned int)index >= TILEDATA_MAX_ID_LAND))
        return NULL;

    struct sPatchResult patch;
    patch.file = NULL;

    if (pVerdataLoader)
        patch = pVerdataLoader->FindPatch(VERDATAPATCH_ART, index);

    struct stIndexRecord idx;

    if (patch.file) {
        idx = patch.index;
    }
    else {
        patch.file = artfile;
        artindex->seekg(index * 12, ios::beg);
        artindex->read((char *)&idx, sizeof(struct stIndexRecord));
    }

    if (idx.offset == 0xffffffff)
        return NULL;

    Uint16 * imagecolors = new Uint16[1024];

    patch.file->seekg(idx.offset, ios::beg);
    patch.file->read((char *)imagecolors, 1024 * 2);

    Texture * texture = new Texture(index);
    texture->Create16(44, 44);
    SDL_Surface * surface = texture->GetSurface();
    //  Uint16 * data = new Uint16[44 * 44];
    //  Uint32 * rdata = new Uint32[44 * 44];
    //  memset (data, 0, 44 * 44 * 2);

    Uint16 * actcol = imagecolors;

    int x = 22;
    int y = 0;
    int linewidth = 2;
    int i, j;

    for (i = 0; i < 22; i++) {
        x--;
        Uint16 * p = ((Uint16 *)surface->pixels) + x + y * 44;
        for (j = 0; j < linewidth; j++) {
            *p = *actcol; p++; actcol++;
        }
        y++;
        linewidth += 2;
    }

    linewidth = 44;
    for (i = 0; i < 22; i++) {
        Uint16 * p = ((Uint16 *)surface->pixels) + x + y * 44;
        for (j = 0; j < linewidth; j++) {
            *p = *actcol;
            p++; actcol++;
        }
        x++;
        y++;
        linewidth -= 2;
    }

    /*	Uint32 col;
        Uint32 * buf = data;
        Uint32 * res = rdata;
        int pos, lw;

        for (y = 1; y <= 22; y++)
        {
            for (x = 0; x < y * 2; x++)
            {
                lw = y * 2 - x - 1;

                pos = (y - 1) * 44 + 22 - y + x;
                col = *(buf + pos);
                *(res + lw * 44 + x) = col;

                if (lw > 0)
                    *(res + (lw - 1) * 44 + x) = col;
            }
        }

        for (y = 43; y >= 22; y--)
        {
            for (x = 0; x < (44 - y) * 2; x++)
            {
                lw = 42 - (43 - y) * 2 + x;

                pos = y * 44 + y - 22 + x;
                col = *(buf + pos);
                *(res + (43 - x) * 44 + lw) = col;

                if (x > 0)
                    *(res + (44 - x) * 44 + lw) = col;
            }
        } */


        //  delete data;
          //delete rdata;
    delete imagecolors;

    return texture;
}


Texture * cArtLoader::LoadStaticArt(int index)
{
    if ((index < TILEDATA_MAX_ID_LAND) || ((unsigned int)index >= art_count))
        return NULL;

    struct sPatchResult patch;
    patch.file = NULL;

    if (pVerdataLoader)
        patch = pVerdataLoader->FindPatch(VERDATAPATCH_ART, index);

    struct stIndexRecord idx;

    if (patch.file) {
        idx = patch.index;
    }
    else {
        patch.file = artfile;
        artindex->seekg(index * 12, ios::beg);
        artindex->read((char *)&idx, sizeof(struct stIndexRecord));
    }

    if (idx.offset == 0xffffffff)
        return NULL;

    Uint8 * filedata = new Uint8[idx.length];
    Uint8 * filep = filedata;

    int header;
    Uint16 width, height;

    patch.file->seekg(idx.offset, ios::beg);
    patch.file->read((char*)filedata, idx.length);
    /*  patch.file->read((char *) &header, 4);
      patch.file->read((char *) &width, 2);
      patch.file->read((char *) &height, 2); */
    header = *((Uint32 *)filep); filep += 4;
    width = *((Uint16 *)filep); filep += 2;
    height = *((Uint16 *)filep); filep += 2;

    //  Uint16 * lookuptable = new Uint16[height];
    if (width & 1) width ++;
    Texture * texture = new Texture(index);
    texture->Create16(width, height);
    SDL_Surface * surface = texture->GetSurface();
    //  Uint16 * data = new Uint16 [width * height];
     // memset(data, 0, width * height * 2);

    Uint16 * lookuptable = (Uint16*)filep;
    //  patch.file->read((char *) lookuptable, height * 2);
    filep += height * 2;

    for (int y = 0; y < height; y++) {
        Uint16 * p = ((Uint16 *)surface->pixels) + y * width;
        Uint16 xOffset = 0, xRun = 0;
        filep = filedata + (lookuptable[y] + height + 4) * 2;
        /*  	patch.file->seekg(idx.offset + (lookuptable[y] + height + 4) * 2, ios::beg);
            patch.file->read((char *) &xOffset, 2);
            patch.file->read((char *) &xRun, 2); */
        xOffset = *((Uint16*)filep); filep += 2;
        xRun = *((Uint16*)filep); filep += 2;
        while ((xOffset != 0) || (xRun != 0)) {
            p += xOffset;
            for (int i = 0; i < xRun; i++) {
                //patch.file->read((char *) &color, 2);
                *p = *((Uint16*)filep) | 0x8000; filep += 2; p++;
            }
            xOffset = *((Uint16*)filep); filep += 2;
            xRun = *((Uint16*)filep); filep += 2;
            /*  		patch.file->read((char *) &xOffset, 2);
                    patch.file->read((char *) &xRun, 2); */
        }

    }

    //  Texture * texture = new Texture;
    //  texture->LoadFromData(data, width, height, 16);

    //  delete lookuptable;
      //delete data;
    delete filedata;

    return texture;
}
