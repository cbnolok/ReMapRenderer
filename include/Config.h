// MAX string length
#include "include.h"
#include <string>

#define RENDER_CACHE_WIDTH 1024
#define RENDER_CACHE_HEIGHT 1024

namespace	nConfig
{
	// UO
    extern int mapindex;
    extern int widthblocks, heightblocks;
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
