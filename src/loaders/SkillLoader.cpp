//
// File: SkillLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "loaders/SkillLoader.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;

cSkillLoader * pSkillLoader = NULL;

cSkillLoader::cSkillLoader (std::string filename, std::string indexname)
{
    string errstr;
    ifstream * skillfile;
    ifstream * skillindex;

    skillfile = new ifstream(filename.c_str(), ios::in | ios::binary);
    if(!skillfile) {
      pDebug.Log("Out of Memory in SkillLoader::SkillLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    skillindex = new ifstream(indexname.c_str(), ios::in | ios::binary);
    if(!skillindex) {
      delete skillfile;
      skillfile = NULL;
      pDebug.Log("Out of Memory in SkillLoader::SkillLoader", __FILE__, __LINE__,
		 LEVEL_ERROR);
      return;
    }
    
    errstr = "Could not load skill file: ";
    if (!skillfile->is_open()) {
	errstr += filename;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete skillfile;
	delete skillindex;
	skillfile = NULL;
	skillindex = NULL;
	return;
    }
    
    if (!skillindex->is_open()) {
	errstr += indexname;
	pDebug.Log((char *)errstr.c_str(), __FILE__, __LINE__, LEVEL_ERROR);
	delete skillfile;
	delete skillindex;
	skillfile = NULL;
	skillindex = NULL;
	return;
    }
      
    skillindex->seekg(0, ios::end);
    unsigned long idxE = skillindex->tellg();
    int skill_count = idxE / 12;
    struct stIndexRecord indexRecord;
    
    skillindex->seekg(0, ios::beg);

    for (int i = 0; i < skill_count; i++) {
	skillindex->read((char *) &indexRecord, 12);
	if ((indexRecord.offset != 0xffffffff) && (indexRecord.length != 0xffffffff)) {
		char * name = (char *) malloc(indexRecord.length);
		Uint8 use_button;
		memset(name, 0, indexRecord.length);
		skillfile->seekg(indexRecord.offset, ios::beg);
		skillfile->read((char*) &use_button, 1);
		skillfile->read(name, indexRecord.length - 1);
		
		cSkillEntry * entry = new cSkillEntry;
		entry->m_name = name;
		entry->use_button = use_button;
		
		skilllist.insert(make_pair((Uint16)(i + 1), entry));
		
		free(name);
	}
    }
        
    delete skillindex;
    delete skillfile;
}

cSkillLoader::~cSkillLoader ()
{
	std::map <Uint16, cSkillEntry *>::iterator iter;
	for (iter = skilllist.begin(); iter != skilllist.end(); iter++)
		delete iter->second;
	skilllist.clear();
}

cSkillEntry * cSkillLoader::GetSkill(Uint16 id)
{
	std::map <Uint16, cSkillEntry *>::iterator iter;
	iter = skilllist.find(id);
	if (iter != skilllist.end())
		return iter->second;
	return NULL;
}
