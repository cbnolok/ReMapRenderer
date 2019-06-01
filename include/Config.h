// MAX string length
#include "include.h"
#include <string>

namespace	nConfig
{
	// GFX
	extern	int		width, height, bpp;

	// UO	
	extern int startx, starty, startz;
	extern std::string mulpath;
	extern int minblockx, maxblockx, minblocky, maxblocky;
	extern int minx, maxx, miny, maxy;
	extern int detail;
	extern int cache_art, cache_texture, cache_ground;
	extern int cache_block;
	extern std::string output;
	extern std::string patches;
	
	// Function
	bool	Init();
};
