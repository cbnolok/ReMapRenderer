//
// File: TextureBuffer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/TextureBuffer.h"
#include "loaders/GroundTextures.h"
#include "loaders/ArtLoader.h"
#include "uotype.h"
#include "Debug.h"
#include "Config.h"

using namespace std;

TextureBuffer * pTextureBuffer = NULL;

TextureBuffer::TextureBuffer()
{
    groundTiles.setMaxSize(nConfig::cache_ground);
    groundTiles.setAutofree(true);

    artTiles.setMaxSize(nConfig::cache_art);
    artTiles.setAutofree(true);

    groundTexmaps.setMaxSize(nConfig::cache_texture);
    groundTexmaps.setAutofree(true);
}

TextureBuffer::~TextureBuffer()
{
}

Texture *TextureBuffer::GetGroundTexture(unsigned int index)
{
    if ((index < 0) || (index >= TILEDATA_MAX_ID_LAND))
        return NULL;

    Texture * result = groundTiles.findEntry(index);

    if (!result) {
        if (!pGroundTextureLoader)
            return NULL;

        result = NULL;//pGroundTextureLoader->LoadTexture(index);

        if (!result) {
            if (!pArtLoader)
                return NULL;
            result = pArtLoader->LoadArt(index);
        }

        if (!result)
            result = new Texture(index);
        groundTiles.addEntry(index, result);
    }

    return result;
}

Texture *TextureBuffer::GetArtTexture(unsigned int index)
{
    if ((index < TILEDATA_MAX_ID_LAND) && (index >= 0))
        return GetGroundTexture(index);

    if ((index < 0x0) || (index >= tiledataMaxID))
        return NULL;

    Texture *result = NULL;

    result = artTiles.findEntry(index);

    if (!result) {

        if (!pArtLoader)
            return NULL;

        result = pArtLoader->LoadArt(index);
        if (!result) {
            result = new Texture(index);
        }
        artTiles.addEntry(index, result);
    }

    return result;
}

Texture *TextureBuffer::GetGroundTexmap(unsigned int index)
{
    if ((index < 0) || (index >= TILEDATA_MAX_ID_LAND))
        return NULL;

    Texture * result = groundTexmaps.findEntry(index);

    if (!result) {
        if (!pGroundTextureLoader)
            return NULL;

        result = pGroundTextureLoader->LoadTexture(index);

        if (!result)
            result = new Texture(index);
        groundTexmaps.addEntry(index, result);
    }

    return result;
}
