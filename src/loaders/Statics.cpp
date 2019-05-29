//
// File: Statics.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "loaders/Statics.h"
#include "Debug.h"
#include "SDL/SDL_image.h"
#include "include.h"
#include "loaders/StaticTextures.h"
#include "loaders/StaticModel.h"

StaticEngine * pStaticEngine = NULL;

using namespace std;

extern bool CheckHeaderID(char *ID1, char *ID2);

struct StaticFileHeader {
  char id[4];
  int modelcount;
  int texturestart;
  int reserved[9];
};

struct StaticFileIndexEntry {
  int index;
  int position;
};

char FILEID_STATIC[] = "CS#2";

StaticEngine::StaticEngine()
{
  textures = NULL;
  staticbuffer = new Buffer();
  staticbuffer->Create(16384);
}

StaticEngine::~StaticEngine()
{
  if(textures)
    delete textures;
  delete staticbuffer;
}

int StaticEngine::Load(char *filename)
{
  struct StaticFileHeader header;
  struct StaticFileIndexEntry *entries;
  StaticModel *model;

  ifstream file(filename, ios::in | ios::binary);

  file.read((char *) &header, sizeof header);

  if(!CheckHeaderID(header.id, FILEID_STATIC)) {
    file.close();
    pDebug.Log("Unknown Header ID in StaticEngine::Load(char *)", __FILE__,
	       __LINE__, LEVEL_WARNING);
    return (false);
  }

  pDebug.Log("    ->loading static textures...");

  file.seekg(header.texturestart, ios::beg);
  textures = new StaticTextures();
  textures->Load(&file);

  file.seekg(sizeof header, ios::beg);

  entries = (struct StaticFileIndexEntry *) malloc(8 * header.modelcount);
  if(!entries) {
    file.close();
    pDebug.Log("Out of Memory in StaticEngine::Load(char *)", __FILE__,
	       __LINE__, LEVEL_ERROR);
    return (false);
  }

  pDebug.Log("    ->loading static models...");
  file.read((char *) entries, 8 * header.modelcount);

  for (int index = 0; index < header.modelcount; index++) {
    file.seekg(entries[index].position, ios::beg);

    model = new StaticModel(entries[index].index);
    if(!model) {
      file.close();
      free(entries);
      pDebug.Log("Out of Memory in StaticEngine::Load(char *)", __FILE__,
		 __LINE__, LEVEL_ERROR);
      return (false);
    }

    model->SetTextures(textures);

    if(!model->Load(&file)) {
      file.close();
      free(entries);
      pDebug.Log("Loading Model in StaticEngine::Load(char *)", __FILE__,
		 __LINE__, LEVEL_ERROR);
      return (false);
    }
    staticbuffer->Add(entries[index].index, model);
  }

  free(entries);

  file.close();
  return true;
}

int StaticEngine::ReLoadTextures(char *filename)
{
  struct StaticFileHeader header;

  ifstream file(filename, ios::in | ios::binary);

  file.read((char *) &header, sizeof header);

  if(!CheckHeaderID(header.id, FILEID_STATIC)) {
    file.close();
    pDebug.Log("Unknown Header ID in StaticEngine::ReLoadTextures(char *)",
	       __FILE__, __LINE__, LEVEL_WARNING);
    return (false);
  }

  pDebug.Log("SYS | reloading static textures...");

  file.seekg(header.texturestart, ios::beg);
  textures = new StaticTextures();
  textures->Load(&file);

  file.close();
  return true;
}

StaticModel *StaticEngine::Get(int index)
{
  (StaticModel *) result = (StaticModel *) staticbuffer->Get(index);	
  if (!result) {
  	std::map<Uint32, bool>::iterator iter = missing_tiles.find(index);
	if (iter == missing_tiles.end()) {
		missing_tiles.insert(make_pair(index, true));
	}
		printf("Tile Missing: %i\n", index);
  }
  return result;
}

int StaticEngine::Paint(int index)
{
  StaticModel *model;

  model = Get(index);

  if(!model)
    return (false);
  model->Paint();

  return (true);
}
