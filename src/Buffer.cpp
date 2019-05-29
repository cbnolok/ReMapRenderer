//
// File: Buffer.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "Buffer.h"
#include "Debug.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

BufferEntry::BufferEntry()
{
}

BufferEntry::~BufferEntry()
{
}

Buffer::Buffer()
{
  size = 0;
  bufferarray = NULL;
}

Buffer::~Buffer()
{
  if(bufferarray)
    Free();
}

int Buffer::GetSize(void)
{
  return size;
}

void Buffer::Create(int size)
{
  if(bufferarray)
    Free();
  this->size = size;
  bufferarray = (BufferEntry **) malloc(size * 4);
  memset(bufferarray, 0, size * 4);
}

void Buffer::Free(void)
{
  if(!bufferarray)
    return;
  Flush();

  free(bufferarray);
  bufferarray = NULL;
  size = 0;
}

void Buffer::Flush(void)
{
  if(!bufferarray)
    return;

  for (int index = 0; index < size; index++)
    if(bufferarray[index])
      delete(bufferarray[index]);

  memset(bufferarray, 0, size * 4);
}

void Buffer::Add(int index, BufferEntry * entry)
{
  if(!bufferarray) {
    pDebug.Log("NULL buffer array in Buffer::Add(int, BufferEntry *)",
	       __FILE__, __LINE__, LEVEL_WARNING);
    return;
  }

  if((index < 0) || (index >= size)) {
    pDebug.Log("Wrong Index in Buffer::Add(int, BufferEntry *)", __FILE__,
	       __LINE__, LEVEL_WARNING);
    return;
  }

  if(bufferarray[index])
    delete bufferarray[index];

  bufferarray[index] = entry;
}

void Buffer::Delete(int index)
{
  Add(index, NULL);
}

BufferEntry *Buffer::Get(int index)
{
  if(!bufferarray) {
    pDebug.Log("NULL buffer array in Buffer::Get(int)", __FILE__, __LINE__,
	       LEVEL_WARNING);
    return NULL;
  }

  if((index < 0) || (index >= size)) {
    return NULL;
  }

  return bufferarray[index];
}
