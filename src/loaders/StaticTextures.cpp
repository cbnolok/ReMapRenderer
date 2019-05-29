//
// File: StaticTextures.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "loaders/StaticTextures.h"
#include "Debug.h"
#include "renderer/Texture.h"
#include "SDL/SDL_image.h"
#include "include.h"

using namespace std;

char FILEID_TEXTURE[] = "CT#2";

struct TextureFileHeader {
  char id[4];
  int count;
  char reserved[40];
};

struct TextureFileEntry {
  char name[60];
  unsigned int colorkey;
  unsigned short AuthorID;
  unsigned short TexID;
};

bool CheckHeaderID(char *ID1, char *ID2)
{
  for (int index = 0; index < 4; index++)
    if(ID1[index] != ID2[index])
      return false;
  return true;
}

StaticTextures::StaticTextures()
{
  count = 0;
  texturearray = NULL;
  namearray = NULL;
}

StaticTextures::~StaticTextures()
{
  Flush();
}

void StaticTextures::Flush(void)
{
  if(texturearray) {
    for (int index = 0; index < count; index++)
      delete texturearray[index];

    free(texturearray);
  }

  if(namearray) {
    for (int index = 0; index < count; index++)
      delete namearray[index].name;

    free(texturearray);
  }
  
  texturearray = NULL;
  namearray = NULL;
  count = 0;

}

int StaticTextures::Load(ifstream * stream)
{
  struct TextureFileHeader header;
  struct TextureFileEntry entry;
  char name[100] = "./data/textures/";

  stream->read((char *) &header, sizeof header);
  if(!CheckHeaderID(header.id, FILEID_TEXTURE)) {
    pDebug.Log("Unknown Header ID in StaticTextures::Load(ifstream*)",
	       __FILE__, __LINE__, LEVEL_WARNING);
    return (false);
  }

  Flush();

  count = header.count;

  namearray = (texcache_entry *) malloc(count * sizeof(struct texcache_entry));
  texturearray = (Texture **) malloc(count * 4);
  if(!texturearray) {
    pDebug.Log("Not Enough Memory in StaticTextures::Load(ifstream*)",
	       __FILE__, __LINE__, LEVEL_CRITICAL);
    return (false);
  }

  memset(texturearray, 0, count * 4);
  memset(namearray, 0, count * sizeof(struct texcache_entry));

  for (int index = 0; index < count; index++) {
    stream->read((char *) &entry, sizeof entry);
    memcpy(&name[16], entry.name, 59);
    name[75] = 0;
//  printf("   Loading %s...\n", name);
    texturearray[index] = NULL;
    
    namearray[index].name = new string;
    *namearray[index].name = string(name);
    namearray[index].colorkey = entry.colorkey;
  }

  return (true);
}

GLuint StaticTextures::Get(int index)
{
  ofstream test;

  if((index >= 0) && (index < count)) {
    Texture * texture = texturearray[index];
    if (!texture) {
      texture = new Texture();
      texture->LoadFromFileWithColorKey(namearray[index].name->c_str(), namearray[index].colorkey);
      texturearray[index] = texture;
    }

    return texture->GetGLTex();
  }
  pDebug.Log("Unknown Texture ID in StaticTextures::Get(int)", __FILE__,
	     __LINE__, LEVEL_WARNING);
  return 0;
}
