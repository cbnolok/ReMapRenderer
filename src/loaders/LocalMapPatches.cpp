//
// File: LocalMapPatches.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include "loaders/LocalMapPatches.h"
#include <fstream>
#include <cstring>
#include "Debug.h"
#include <stdlib.h>

extern bool CheckHeaderID(char *ID1, char *ID2);

char FILEID_PATCHES[] = "CP#1";

struct PatchFileHeader {
  char id[4];
  int StaticPatchCount;
  int StaticPatchStart;
  int MapPatchCount;
  int MapPatchStart;
  int reserved[9];
};


bool CheckHeaderID(char *ID1, char *ID2)
{
  for (int index = 0; index < 4; index++)
    if(ID1[index] != ID2[index])
      return false;
  return true;
}

using namespace std;

LocalMapPatches::LocalMapPatches ()
{
	root = NULL;
	count = 0;
}


LocalMapPatches::~LocalMapPatches ()
{
	Clear();
}

void LocalMapPatches::Clear()
{

   struct PatchEntry * patch;
   while (root)
   {
      patch = root;
	  root = root->next;

       free (patch);
   }

   root = NULL;

   count = 0;
}

void LocalMapPatches::AddPatch(struct StaticPatch patch)
{
   struct PatchEntry * entry = (struct PatchEntry *) malloc (sizeof (struct PatchEntry));

   if (!entry) {
      pDebug.Log("Out Of Memory in AddPatch::Add(struct StaticPatch)", __FILE__, __LINE__, LEVEL_CRITICAL);
      return;
   }

   memset (entry, 0, sizeof (struct PatchEntry));

   entry->patch = patch;
   entry->next = root;
   root = entry;

   count ++;
}

void LocalMapPatches::DelPatch(unsigned int blockx, unsigned int blocky, unsigned int obj_id)
{
  struct PatchEntry *entry = root;
  struct PatchEntry *last = NULL;


  while (entry) {
      if ((entry->patch.blockx == blockx) && (entry->patch.blocky == blocky) && (entry->patch.obj_id == obj_id))
      break;
    last = entry;
    entry = entry->next;
  }
  if (entry) {
  	if (last) {
		last->next = entry->next;
		free(entry);
	} else {
		entry = entry->next;
		free (root);
		root = entry;
	}
	count--;
  }
}

int LocalMapPatches::GetNewStaticSize(int orig_count, unsigned int blockx, unsigned int blocky, bool &apply_needed)
{
   int result = orig_count;
   apply_needed = false;

   struct PatchEntry * entry = root;
   while (entry)
   {
	struct StaticPatch * patch = &entry->patch;
   	if ((patch->blockx == blockx) && (patch->blocky == blocky))
	{
		apply_needed = true;
		if (patch->type == PATCH_ADD)
			result ++;

		if (patch->type == PATCH_DELETE)
			result --;
	}
   	entry = entry->next;
   }
   return result;
}


int LocalMapPatches::GetNewID(int blockx, int blocky)
{
   unsigned int result = 0x10000000;

   struct PatchEntry * entry = root;
   while (entry)
   {
	struct StaticPatch * patch = &entry->patch;
   	if ((patch->blockx == blockx) && (patch->blocky == blocky))
		if (patch->obj_id > result)
			result = patch->obj_id;

   	entry = entry->next;
   }
   return result + 1;
}


StaticPatch * LocalMapPatches::GetPatch(unsigned int blockx, unsigned int blocky, unsigned int objid)
{
   struct PatchEntry * entry = root;
   while (entry)
   {
	struct StaticPatch * patch = &entry->patch;
   	if ((patch->blockx == blockx) && (patch->blocky == blocky) && (patch->obj_id == objid))
		return patch;

   	entry = entry->next;
   }
   return NULL;
}


int LocalMapPatches::Load(char *filename)
{
  struct PatchFileHeader header;

  ifstream file(filename, ios::in | ios::binary);
  
  if(!file.is_open()) {
    string s = "File not found (" + string(filename) + ") in LocalMapPatches::Load(char *)";
    pDebug.Log((char*) s.c_str(), __FILE__,
	       __LINE__, LEVEL_WARNING);
    return (false);
  }

  file.read((char *) &header, sizeof header);

  if(!CheckHeaderID(header.id, FILEID_PATCHES)) {
    file.close();
    pDebug.Log("Unknown Header ID in LocalMapPatches::Load(char *)", __FILE__,
	       __LINE__, LEVEL_WARNING);
    return (false);
  }

  Clear();

  struct StaticPatch * patches = (struct StaticPatch *) malloc(header.StaticPatchCount * sizeof (struct StaticPatch));
  file.seekg(header.StaticPatchStart, ios::beg);
  file.read((char *) patches, header.StaticPatchCount * sizeof (struct StaticPatch));

  for (int index = 0; index < header.StaticPatchCount; index++)
  	AddPatch(patches[index]);

  free(patches);

  file.close();
  pDebug.Log("Using " + string(filename) + "...");
  return true;
}


int LocalMapPatches::Save(char *filename)
{
  struct PatchFileHeader header;

  ofstream file(filename, ios::out | ios::binary);

  memset(&header, 0, sizeof(header));

  memcpy (header.id, FILEID_PATCHES, 4);
  header.StaticPatchCount = count;
  header.StaticPatchStart = sizeof(header);

  file.write((char*) &header, sizeof header);


  struct PatchEntry * entry = root;

  while (entry) {
	  file.write((char *) &entry->patch, sizeof (struct StaticPatch));
	  entry = entry->next;
  }

  file.close();
  return true;
}


void LocalMapPatches::ApplyPatch(unsigned int blockx, unsigned int blocky, unsigned int oldcount,  unsigned int newcount, struct staticinfo * src, struct staticinfo * dst)
{
	bool * delete_list = (bool *) malloc (oldcount);
	memset(delete_list, 0, oldcount);


 	struct PatchEntry * entry = root;
  	while (entry)
  		{
			struct StaticPatch * patch = &entry->patch;
   			if ((patch->blockx == blockx) && (patch->blocky == blocky)) {
				if (patch->type == PATCH_DELETE) {
					delete_list[patch->obj_id] = true;
				}
				else { /* PATCH_ADD */
						dst->TileID = patch->tileid;
						dst->x = patch->x;
						dst->y = patch->y;
						dst->z = patch->z;
						dst->obj_id = patch->obj_id;
						dst++;

				}
			}

	   	entry = entry->next;
   	}


	bool * p = delete_list;

	for (unsigned int index = 0; index < oldcount; index++)  {
		if (!*p)
			{
				*dst = *src;
				dst++;
			}
		p++;
		src++;
	}


	free(delete_list);
}

