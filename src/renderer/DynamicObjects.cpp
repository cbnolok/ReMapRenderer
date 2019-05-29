//
// File: DynamicObjects.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/DynamicObjects.h"
#include "include.h"

using namespace std;
  
cDynamicObject::cDynamicObject ()
{
	id = 0;
	x = 0; y = 0; z = 0;
	model = 0;
	itemcount = 0;
	incrcounter = 0;
	direction = 0;
	dye = 0;
	flag = 0;
	clip = 0;
	type = 0;
	parent = 0;
}


cDynamicObject::~cDynamicObject ()
{
}

cDynamicObjectList * pDynamicObjectList = NULL;


cDynamicObjectList::cDynamicObjectList ()
{
}


cDynamicObjectList::~cDynamicObjectList ()
{
	Clear();
}


void cDynamicObjectList::Clear()
{
	dynamiclist_t::iterator iter;
	for (iter = dynamiclist.begin(); iter != dynamiclist.end(); iter++)
		delete iter->second;
	dynamiclist.clear();
}


cDynamicObject * cDynamicObjectList::Add (Uint32 id) 
{
	dynamiclist_t::iterator iter;
	iter = dynamiclist.find(id);

	if (iter != dynamiclist.end()) 
		return iter->second; 

	cDynamicObject * result = new cDynamicObject;
	dynamiclist.insert(std::make_pair(id, result));
	result->id = id;
	return result;
}
 
  
cDynamicObject * cDynamicObjectList::Get (unsigned int id)
{
	dynamiclist_t::iterator iter;
	iter = dynamiclist.find(id);

	if (iter != dynamiclist.end()) 
		return iter->second; 
	else
		return NULL;
}

void cDynamicObjectList::Delete (unsigned int id)
{
	delete Get(id);
	dynamiclist.erase(id);
}
