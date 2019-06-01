
#if !defined( __HUELOADER_H__ )
#define __HUELOADER_H__

#include <map>
#include <string>

struct stHue
{
    unsigned int colors[32];
    unsigned int tableStart;
    unsigned int tableEnd;
};

class cHueLoader
{
private:
    std::map< unsigned short, stHue > hues;
    bool load(std::string filename);

public:
    cHueLoader(std::string filename);
    virtual ~cHueLoader();

    const stHue *getHue(unsigned short id);
};

extern cHueLoader * pHueLoader;

#endif
