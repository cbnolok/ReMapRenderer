//
// File: VerdataLoader.cpp
// Created by:  Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/VerdataLoader.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;

cVerdataLoader * pVerdataLoader = NULL;

cVerdataLoader::cVerdataLoader (std::string filename)
{
    patch_count = 0;

    string errstr;

    verdatafile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!verdatafile) {
      pDebug.Log("Out of Memory in VerdataLoader::VerdataLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    

    errstr = "Could not load verdata file: ";
    if (!verdatafile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete verdatafile;
	verdatafile = NULL;
	return;
    }
    

    verdatafile->seekg(0, ios::beg);
    verdatafile->read((char *) &patch_count, 4);
    patches = (struct VerDataEntry *) malloc (patch_count * sizeof (struct VerDataEntry));
    verdatafile->read((char *) patches, patch_count * sizeof (struct VerDataEntry));
}

cVerdataLoader::~cVerdataLoader ()
{
	delete verdatafile;
	free(patches);
}

struct sPatchResult cVerdataLoader::FindPatch (unsigned int fileid, unsigned int blockid)
{
	struct sPatchResult result;
	result.file = NULL;
	
	for (Uint32 i = 0; i < patch_count; i++)
		if ((patches[i].fileid == fileid) && (patches[i].blockid == blockid)) {
			result.index.offset = patches[i].start;
			result.index.length = patches[i].len;
			result.index.extra = patches[i].extra;
			result.file = verdatafile;
		}
	
	return result;
}
