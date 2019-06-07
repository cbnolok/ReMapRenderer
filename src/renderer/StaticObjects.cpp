//
// File: StaticObjects.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <algorithm>
#include "renderer/StaticObjects.h"
#include "Config.h"

using namespace std;

cStaticObject::cStaticObject()
{
    id = 0;
    height = 0;
    tiledata_flags = 0;
    tileclass = TILE_CLASS_ITEM;
}

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

static inline bool TestObject (const cEntity * object1, const cEntity * object2)
{
#define HAS_TF(x,y)     (x->tiledata_flags & y)
#define DIFFERENT_TF(y) (HAS_TF(object1,y) != HAS_TF(object2,y))
#define TF_1BELOW(y)    (!HAS_TF(object1, y) && HAS_TF(object2, y))

    if (nConfig::render_underground)
    {
        if (object1->tileclass != object2->tileclass)
            return (object1->tileclass < object2->tileclass);
    }
    
    if (object1->x != object2->x)
        return (object1->x < object2->x);
    if (object1->y != object2->y)
        return (object1->y < object2->y);

    if (object1->z != object2->z)
        return (object1->z < object2->z);

    if (DIFFERENT_TF(TDStaticFlag::Foliage))
        return TF_1BELOW(TDStaticFlag::Foliage);
    /*if (DIFFERENT_TF(TDStaticFlag::Background))
        return !TF_1BELOW(TDStaticFlag::Background);
    if (DIFFERENT_TF(TDStaticFlag::Surface))
        return !TF_1BELOW(TDStaticFlag::Surface);*/
    if (object1->height != object2->height)
        return (object1->height < object2->height);

    return false;

#undef HAS_TF
#undef DIFFERENT_TF
#undef TF_1BELOW
}

void StaticObjectList::Sort(void) {
    std::sort(objectlist.begin(), objectlist.end(), TestObject);
}

/*
void StaticObjectList::InsertDynamic (cStaticObject * object)
{
    for (objectlist_t::iterator iter = objectlist.begin(), iterend = objectlist.end(); iter != iterend; ++iter) {
        if (!TestObject(object, *iter)) {
            objectlist.insert(iter, object);
            return;
        }
    }
    objectlist.push_back(object);

}

void StaticObjectList::DeleteDynamic (Uint32 id)
{
}
*/
