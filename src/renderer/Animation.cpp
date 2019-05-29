//
// File: Animation.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Animation.h"
#include "include.h"

using namespace std;

cAnimation::cAnimation ()
{
}

cAnimation::~cAnimation ()
{
    ClearFrames();
}
    
void cAnimation::ClearFrames()
{
    for (int i=0; i < frames.size(); i++)
        delete frames[i];
    frames.clear();    
}

sAnimationFrame * cAnimation::AddFrame()
{
    sAnimationFrame * result = new sAnimationFrame;
    memset(result, 0, sizeof (struct sAnimationFrame));
    frames.push_back(result);
    return result;
}

sAnimationFrame * cAnimation::GetFrame(Uint32 frame)
{
    if (frame < frames.size())
        return frames[frame];
}
