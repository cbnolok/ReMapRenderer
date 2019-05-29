
#if !defined( __CACHE_H__ )
#define __CACHE_H__

#include <vector>
#include <map>

template< class C >
class Cache
{
private:
	std::vector< unsigned int > cache_history;
	C ** cache;
	unsigned short cachesize; // How many elements are available

	unsigned int _maxsize;
	bool _autofree;
public:
	
	void addEntry( unsigned int id, C *data )
	{
		if( id >= cachesize )
			return;

		while( cache_history.size() > _maxsize )
		{
			unsigned int id = *( cache_history.begin() );
			
			if( _autofree )
				delete cache[id];

			cache[id] = 0;
			cache_history.erase( cache_history.begin() );
		}

		cache[id] = data;
		cache_history.push_back( id );
	}

	C *findEntry( unsigned int id )
	{
		if( id >= cachesize )
			return 0;

		return cache[id];
	}

	void setCacheSize( unsigned short elements ) 
	{
		if( autofree() )
		{
			for( unsigned int i = 0; i < cachesize; ++i )
				delete cache[i];
		}

		delete cache;

		cachesize = elements;
		cache = new C*[ elements ];
		memset( cache, 0, elements * sizeof( C* ) );
   	}

	Cache()
	{
		_maxsize = 1;

		cachesize = 0;
		cache = 0;
	}

	void setMaxSize( unsigned int data ) { _maxsize = data; }
	unsigned int maxSize() const { return _maxsize; }

	virtual ~Cache()
	{
		if( _autofree )
		{
			for( unsigned int i = 0; i < cachesize; ++i )
				delete cache[i];
		}
	}

	void setAutofree( bool data ) { _autofree = true; }
	bool autofree() const { return _autofree; }
};

#endif
