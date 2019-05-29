
#include "loaders/FontLoader.h"
#include "Debug.h"
#include "Config.h"
#include "uotype.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

cFontLoader::cFontLoader(string filename)
{
	load(filename);
}

cFontLoader::~cFontLoader()
{
  std::map < unsigned short, stFont >::const_iterator it;

  for (it = fonts.begin(); it != fonts.end(); ++it) {
    unsigned int i;

    for (i = 0; i < 224; ++i) {
      delete[]it->second.chars[i].pixels;
      delete[]it->second.chars[i].redmask;
    }
  }
}

// Load Fonts
bool cFontLoader::load(string filename)
{
  std::ifstream fontfile;
  fontfile.open(filename.c_str(), std::ios::in | std::ios::binary);

  if(!fontfile.is_open()) {
    pDebug.Log("Couldn't open fonts.mul in cFontLoader::load();", __FILE__,
	       __LINE__, LEVEL_ERROR);
    return false;
  }

  unsigned int j = 0;

  // Read as many fonts as we can
  while (!fontfile.eof()) {
    // Header
    unsigned char header;

    fontfile.get(reinterpret_cast<char&>(header));

    // The only valid header in found is an one
    if(header != 1)
      break;

    // Read in the 224 characters
    unsigned int i;
    stFont font;

    font.maxHeight = 0;

    for (i = 0; i < 224; ++i) {
      // Width + Height + Header
      fontfile.get(reinterpret_cast<char&>(font.chars[i].width));
      fontfile.get(reinterpret_cast<char&>(font.chars[i].height));
      fontfile.get(reinterpret_cast<char&>(font.chars[i].header));

      if(font.chars[i].height > font.maxHeight)
	font.maxHeight = font.chars[i].height;

      // Real Character Data
      font.chars[i].pixels =
	new unsigned int[font.chars[i].width * font.chars[i].height];
      font.chars[i].redmask =
	new unsigned char[font.chars[i].width * font.chars[i].height];

      unsigned short *pixels =
	new unsigned short[font.chars[i].width * font.chars[i].height];
      fontfile.read(reinterpret_cast<char*>(pixels),
		    font.chars[i].width * font.chars[i].height * 2);

      for (int j = 0; j < font.chars[i].width * font.chars[i].height; ++j) {
	if(pixels[j] == 0) {
	  font.chars[i].pixels[j] = 0;
	  font.chars[i].redmask[j] = 0;
	} else {
	  font.chars[i].pixels[j] = color15to32(pixels[j]);
	  font.chars[i].redmask[j] = (pixels[j] >> 10) & 0x1F;
	}
      }

      delete[]pixels;
    }

    fonts.insert(std::make_pair(j++, font));
  }

  fontfile.close();

  return true;
}

const stFont * cFontLoader::getFont(unsigned short id) const
{
  std::map < unsigned short, stFont >::const_iterator it = fonts.find(id);

  if(it == fonts.end())
    return NULL;

  return &(it->second);
}

cFontLoader *pFontLoader = NULL;
