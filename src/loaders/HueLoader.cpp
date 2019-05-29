
#include "loaders/HueLoader.h"
#include "Debug.h"
#include "Config.h"
#include "uotype.h"

#include <iostream>
#include <fstream>
#include <string>

cHueLoader::cHueLoader(std::string filename)
{
	load(filename);
}

cHueLoader::~cHueLoader()
{
}

bool cHueLoader::load(std::string filename)
{
  std::ifstream huefile;
  huefile.open(filename.c_str(), std::ios::in | std::ios::binary);

  if(!huefile.is_open()) {
    pDebug.Log("Couldn't open hues.mul in cHueLoader::load();", __FILE__,
	       __LINE__, LEVEL_ERROR);
    return false;
  }

  unsigned int i = 0;

  // Read as many fonts as we can
  while (!huefile.eof()) {
    huefile.seekg(4, std::ios::cur);

    for (unsigned int j = 0; j < 8; ++j) {
      unsigned short hueId = i * 8 + j;
      
      stHue hue;

      // 32 Color Values
      for (unsigned int k = 0; k < 32; ++k) {
	unsigned short color15;

	huefile.read((char *) &color15, 2);

	// Swap Bytes to 32 bits
	hue.colors[k] = color15;//color15to32(color15);
      }

      // Two other bytes, tablestart and tableend
      unsigned short tableStart, tableEnd;

      huefile.read((char *) &tableStart, 2);
      huefile.read((char *) &tableEnd, 2);

      hue.tableEnd = color15to32(tableEnd);
      hue.tableStart = color15to32(tableStart);

      hues.insert(std::make_pair(hueId, hue));

      // Skip the Name
      huefile.seekg(20, std::ios::cur);
    }

    i++;

  }

  huefile.close();

  return true;
}

const stHue *cHueLoader::getHue(unsigned short id)
{
  std::map < unsigned short, stHue >::iterator it = hues.find(id);

  if(it == hues.end())
    return 0;

  return &(it->second);
}

cHueLoader * pHueLoader = NULL;
