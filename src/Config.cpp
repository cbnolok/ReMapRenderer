#include <string>
#include "Debug.h"
#include "xml.h"
using namespace std;

	#define IS_SECTION 1
	#define IS_INTEGER 2
	#define IS_STRING  3
	#define IS_BOOL    4
	#define IS_END     5

struct ParserData{
	ParserData ( const string & NewName,int NewType,void * NewDataPtr) : name(NewName) , type(NewType) , dataptr(NewDataPtr){};
	string name;
	int type;
	void * dataptr; // datatofill -> yes void pointers are bad style.
};
	

namespace nConfig {

// Screen related
  int width = 640;
  int height = 480;
  int bpp = 16;
  int startfullscreen = 0;

// Net related
  string server = "localhost";
  string login = "account";
  string password = "password";
  string output="output.bmp";
  int serverport = 2593;

// UO related
  int startx = 1500;
  int starty = 1000; 
  int startz = 0;
  string mulpath;
  string patches = "";

  int minblockx = 344; 
  int maxblockx = 364;
  int minblocky = 222;
  int maxblocky = 242;

  int minx = 1880; 
  int miny = 2660;
  int maxx = 1900;
  int maxy = 2680;
  
  // size in bytes
  int cache_art = 500000;
  int cache_texture = 200000;
  int cache_ground = 1000000;

  // number of blocks
  int cache_block = 1000;

  int detail = 0;
}
using namespace nConfig;
// important always write childs of section behind that Section array element
ParserData ParserInfo[] = {
	// Section Gfx
	ParserData("GFX", IS_SECTION , NULL),
	ParserData("WIDTH", IS_INTEGER , &width),
	ParserData("HEIGHT", IS_INTEGER , &height),
	ParserData("BPP", IS_INTEGER , &bpp),
	ParserData("FULLSCREEN", IS_INTEGER , &startfullscreen),
	ParserData("UO", IS_SECTION , NULL),
	ParserData("MINX", IS_INTEGER , &minx),
	ParserData("MINY", IS_INTEGER , &miny),
	ParserData("MAXX", IS_INTEGER , &maxx),
	ParserData("MAXY", IS_INTEGER , &maxy),
	ParserData("STARTX", IS_INTEGER , &startx),
	ParserData("STARTY", IS_INTEGER , &starty),
	ParserData("STARTZ", IS_INTEGER , &startz),
	ParserData("MULPATH", IS_STRING , &mulpath),
	ParserData("OUTPUT", IS_STRING , &output),
	ParserData("PATCHES", IS_STRING , &patches),
	ParserData("DETAIL", IS_INTEGER , &detail),
	ParserData("CACHE_ART", IS_INTEGER , &cache_art),
	ParserData("CACHE_TEXTURE", IS_INTEGER , &cache_texture),
	ParserData("CACHE_GROUND", IS_INTEGER , &cache_ground),
	ParserData("CACHE_BLOCK", IS_INTEGER , &cache_block),
	ParserData("NET", IS_SECTION , NULL),
	ParserData("PORT", IS_INTEGER , &serverport),
	ParserData("SERVER", IS_STRING , &server),
	ParserData("LOGIN", IS_STRING , &login),
	ParserData("PASSWORD", IS_STRING , &password),
	ParserData("", IS_END , NULL)
};
namespace nConfig{
	
// Functions
	bool Init() {
	  XML::Parser parser;
	  XML::Node *config, *document;
	  
	  try
	  {
		  parser.loadData( "config.xml" );
		  document = parser.parseDocument();
		  
		  config = document->findNode( "CONFIG" );
		  
		  if( !config )
			  throw "Couldn't find configuration node.";
	  }
	  catch( ... )
	  {
		  //pDebug(Debug::Critical) << "Couldn't parse the configuration file. Please reinstall Ultima Iris.\n";
		  return false;
      }
	  XML::Node * section = NULL, * value = NULL;
	  for(int i = 0;ParserInfo[i].type != IS_END;i++){
		  const ParserData & Data = ParserInfo[i];
		  if(Data.type == IS_SECTION){
			  section = config->findNode(Data.name);
		  }
		  else{
			  value = section != NULL ? section->findNode( Data.name ) : config->findNode( Data.name ); // If no section is loaded get from <config>
			  if(value == NULL) ;//pDebug(Debug::Warning) << "Couldn't load " << Data.name << "\n";
			  else{
				  if(Data.type == IS_BOOL) *reinterpret_cast<bool*>(Data.dataptr) = value->asBool();
				  else if(Data.type == IS_INTEGER) *reinterpret_cast<int*>(Data.dataptr) = value->asInteger();
				  else if(Data.type == IS_STRING) *reinterpret_cast<string*>(Data.dataptr) = value->asString();
			  }
		  }		  
	  }
	  delete document ; // I don't like that I'll alter darkstorm's class xml class so this is not necessary
	  
	  minblockx = minx / 8;
	  minblocky = miny / 8;
	  maxblockx = maxx / 8;
	  maxblocky = maxy / 8;
	  
	  return true;
  }
}
