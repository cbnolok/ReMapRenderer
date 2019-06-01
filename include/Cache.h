
#if !defined( __CACHE_H__ )
#define __CACHE_H__

#include <map>

template< class C >
class Cache
{
private:
	C ** cache;
	unsigned short cachesize; // How many elements are available

    std::map< unsigned int, C * > _cache_history; // pair<tile ID, cache entry *>
	bool _autofree;
    unsigned int _maxsize;
    unsigned int _insertposition;
public:
	
	void addEntry( unsigned int id, C *data )
	{
		if ((!_autofree && (cachesize >= _maxsize)) || (_cache_history.find(id) != _cache_history.end()) )
			return;

        if (cachesize >= _maxsize)
        {
            if (_insertposition >= _maxsize)
                _insertposition = 0;
            _cache_history.erase(cache[_insertposition]->GetId());
            delete cache[_insertposition];
        }
        else
        {
            ++cachesize;
        }
        cache[_insertposition] = data;
        _cache_history.emplace(id, data);
        ++_insertposition;
	}

	C *findEntry( unsigned int id )
	{
        auto it_entry = _cache_history.find(id);
		if( it_entry == _cache_history.end() )
			return 0;

		return it_entry->second;
	}

	void setMaxSize( unsigned int elements ) 
	{
		if( autofree() )
		{
			for( unsigned int i = 0; i < cachesize; ++i )
				delete cache[i];
		}

		delete cache;

        cachesize = 0;
        _insertposition = 0;
        _cache_history.clear();
        _maxsize = elements;
		cache = new C*[ elements ];
		memset( cache, 0, elements * sizeof( C* ) );
   	}

	Cache()
	{
		_maxsize = 1;
        _insertposition = 0;

		cachesize = 0;
		cache = 0;
	}

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
