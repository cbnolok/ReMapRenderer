//
// File: Node.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Node.h"
#include "include.h"

using namespace std;


cNode::cNode(Uint32 id)
{
    this->id = id;
    vertex[0] = ((int)(id >> 21) - 512) * 0.05f;
    vertex[1] = ((int)((id >> 10) & 2047) - 512) * 0.05f;
    vertex[2] = ((int)(id & 1023) - 512) * 0.25f;
}

cNode::~cNode()
{
}

void cNode::AddQuad(void * quad)
{
    quads.push_back(quad);
}

cNodeList::cNodeList()
{
}

cNodeList::~cNodeList()
{
    Clear();
}

void cNodeList::Clear(void)
{
    nodelist_t::iterator iter;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            for (iter = nodes[i][j].begin(); iter != nodes[i][j].end(); iter++)
                delete(*iter).second;

            nodes[i][j].clear();
        }
}


cNode * cNodeList::AddNode(float x, float y, float z)
{
    int nx = (int)(x / 2.0f);
    int ny = (int)(y / 2.0f);
    if (nx < 0) nx = 0;
    if (nx > 3) nx = 3;
    if (ny < 0) ny = 0;
    if (ny > 3) ny = 3;

    int rx = (int)((x * 20.0f) + 512.5f);
    if ((rx < 0) || (rx > 2047))
        return NULL;

    int ry = (int)((y * 20.0f) + 512.5f);
    if ((ry < 0) || (ry > 2047))
        return NULL;

    int rz = (int)((z * 4.0f) + 512.5f);
    if ((rz < 0) || (rz > 1023))
        return NULL;

    Uint32 id = ((Uint32)rx) << 21 | ((Uint32)ry) << 10 | ((Uint32)rz);

    nodelist_t::iterator iter;

    iter = nodes[ny][nx].find(id);

    if (iter != nodes[ny][nx].end())
        return (*iter).second;

    cNode * result = new cNode(id);
    nodes[ny][nx].insert(make_pair(id, result));

    return result;
}


cNodeQuadList::cNodeQuadList()
{
    _transparent = false;
}

cNodeQuadList::~cNodeQuadList()
{
    Clear();
}


void cNodeQuadList::Clear()
{
    nodequadlist_t::iterator iter;

    for (iter = quads.begin(); iter != quads.end(); iter++)
        delete(*iter);

    quads.clear();
}


sNodeQuad * cNodeQuadList::AddQuad()
{

    static Uint32 id = 0;
    id ++;

    sNodeQuad * result = new sNodeQuad;
    if (!result) {
        return NULL;
    }

    memset(result, 0, sizeof (sNodeQuad));
    result->id = id;

    quads.push_back(result);

    return result;
}

/*
struct functor_ltszi
{
 bool operator()(sNodeQuad * quad1, sNodeQuad * quad2) const
 {
 return quad1->texture < quad2->texture;
 }
};

void cNodeQuadList::Sort()
{
// quads.sort (functor_ltszi ());
}
*/

