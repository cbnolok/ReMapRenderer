//
// File: AnimBuffer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/AnimBuffer.h"
#include "loaders/AnimLoader.h"
#include "Debug.h"

using namespace std;

AnimBuffer * pAnimBuffer = NULL;

AnimBuffer::AnimBuffer()
{
  animations.setMaxSize(500);
  animations.setCacheSize(65500);
  animations.setAutofree(true);
}

AnimBuffer::~AnimBuffer()
{
}

cAnimation * AnimBuffer::GetAnimation(int index)
{

  if ((index < 0) || (index >= 0x10000))
  	return NULL;

  cAnimation * result = animations.findEntry(index);

  if(!result) {
    if (!pAnimLoader)
    	return NULL;
	
  	result = pAnimLoader->LoadAnimation(index);
    
    if (!result)
    	result = new cAnimation;
    animations.addEntry(index, result);
  }

  return result;
}



