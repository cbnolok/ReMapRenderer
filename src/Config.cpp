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

// UO related
  int mapindex = 0;
  int widthblocks = 768;
  int heightblocks = 512;

  string mulpath;
  string patches = "";
  string output="output.bmp";

  int minblockx = -1; 
  int maxblockx = -1;
  int minblocky = -1;
  int maxblocky = -1;

  int minx = 1880; 
  int miny = 2660;
  int maxx = 1900;
  int maxy = 2680;
  
  // maximum number of elements in the cache
  int cache_art     = 500;  // art tile SDL texture
  int cache_texture = 300;  // ground tile SDL texture
  int cache_ground  = 1000; // ground texmaps
  int cache_block = 150;    // map blocks

  int detail = 0;
}
using namespace nConfig;
// important always write childs of section behind that Section array element
ParserData ParserInfo[] = {
	ParserData("UO", IS_SECTION , NULL),
    ParserData("MAPINDEX", IS_INTEGER , &mapindex),
    ParserData("WIDTHBLOCKS", IS_INTEGER , &widthblocks),
    ParserData("HEIGHTBLOCKS", IS_INTEGER , &heightblocks),
	ParserData("MINX", IS_INTEGER , &minx),
	ParserData("MINY", IS_INTEGER , &miny),
	ParserData("MAXX", IS_INTEGER , &maxx),
	ParserData("MAXY", IS_INTEGER , &maxy),
	ParserData("MULPATH", IS_STRING , &mulpath),
	ParserData("OUTPUT", IS_STRING , &output),
	ParserData("PATCHES", IS_STRING , &patches),
	ParserData("DETAIL", IS_INTEGER , &detail),
	ParserData("CACHE_ART", IS_INTEGER , &cache_art),
	ParserData("CACHE_TEXTURE", IS_INTEGER , &cache_texture),
	ParserData("CACHE_GROUND", IS_INTEGER , &cache_ground),
	ParserData("CACHE_BLOCK", IS_INTEGER , &cache_block),
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
