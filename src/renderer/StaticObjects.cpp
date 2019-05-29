//
// File: StaticObjects.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <algorithm>
#include "renderer/StaticObjects.h"
#include "include.h"

using namespace std;


cGroundObject::cGroundObject ()
{
    tileid = 0;
    height = 0;
    tileclass = TILE_CLASS_GROUND;
    stretch = 0;
    hue = 0;
}

cGroundObject::~cGroundObject ()
{
}

   void StaticObjectList::Clear (void) {
    for (cEntity * elem : objectlist)
		delete elem;
	objectlist.clear();
   	
   }
   
   cStaticObject * StaticObjectList::AddStatic (void) {
   	cStaticObject * result = new cStaticObject;
	objectlist.push_back(result);
	return result;
}

   cGroundObject * StaticObjectList::AddGround(void) {
   	cGroundObject * result = new cGroundObject;
	objectlist.push_back(result);
	return result;
  }

static inline bool TestObject (cEntity * object1, cEntity * object2)
{
  if (object1->tileclass != object2->tileclass) 
    return (object1->tileclass < object2->tileclass);
  if (object1->x != object2->x) 
    return (object1->x < object2->x);
  if (object1->y != object2->y) 
    return (object1->y < object2->y); 
  if (object1->z != object2->z)
    return (object1->z < object2->z);
  if (object1->height != object2->height)
    return object1->height < object2->height;
  return false;
}

void StaticObjectList::Sort(void) {
  std::sort(objectlist.begin(), objectlist.end(), TestObject);
}

void StaticObjectList::InsertDynamic (cStaticObject * object)
{
   	for (objectlist_t::iterator iter = objectlist.begin(), iterend = objectlist.end(); iter != iterend; ++iter) {
   	    if (!TestObject(object, *iter) ) {
   	            objectlist.insert(iter, object);
   	            return;
        }
   	}
    objectlist.push_back(object);
    
}

void StaticObjectList::InsertCharacter (cStaticCharacter * object)
{
    printf("Act_Char\n");
/*  for (objectlist_t::iterator iter = objectlist.begin(), iterend = objectlist.end(); iter != iterend; ++iter) {
   	    if (!TestObject(object, *iter) ) {
   	            objectlist.insert(iter, object);
   	            return;
        }
   	} */
    objectlist.push_back(object);
    
}

void StaticObjectList::DeleteDynamic (Uint32 id)
{
}

