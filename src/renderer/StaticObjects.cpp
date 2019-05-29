//
// File: StaticObjects.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
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
   	objectlist_t::iterator iter;
   	for (iter = objectlist.begin(); iter != objectlist.end(); iter++)
		delete (*iter);
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

bool TestObject (cEntity * object1, cEntity * object2)
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

struct functor_ltszi
{
 bool operator()(cEntity * object1, cEntity * object2) const
 {
 return TestObject(object1, object2);
 } 
};


void StaticObjectList::Sort(void) {
  objectlist.sort (functor_ltszi ());
}

void StaticObjectList::InsertDynamic (cStaticObject * object)
{
   	objectlist_t::iterator iter;
   	for (iter = objectlist.begin(); iter != objectlist.end(); iter++) {
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
/*   	objectlist_t::iterator iter;
   	for (iter = objectlist.begin(); iter != objectlist.end(); iter++) {
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

