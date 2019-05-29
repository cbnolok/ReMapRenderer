
#if !defined( __FONTLOADER_H__ )
#define __FONTLOADER_H__

#include <map>
#include <string>

struct stChar
{
	unsigned char width;
	unsigned char height;
	unsigned char header; // Unknown
	unsigned char *redmask; // red mask pixel data (for hueing only)
	unsigned int *pixels; // Pixel Data
};

struct stFont
{
	unsigned char maxHeight;
	stChar chars[224]; // The first 32 characters are NOT included (control chars)
};

class cFontLoader
{
private:
	std::map< unsigned short, stFont > fonts;
	bool load(std::string filename);
public:
	cFontLoader(std::string filename);
	virtual ~cFontLoader();

	const stFont *getFont( unsigned short id ) const;
};

extern cFontLoader * pFontLoader;

#endif
