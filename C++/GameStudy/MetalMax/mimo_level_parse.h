#ifndef _MIMO_LEVEL_PARSE_
#define _MIMO_LEVEL_PARSE_

#include "mimo_include.h"
#include "mimo_level.h"
#include "mimo_layer.h"
#include "tinyxml.h"

namespace mimo{

	class level_parse
	{
	public:
		game_level * parse_level(const char * filename);
	private:
		void parse_tileset(TiXmlElement * ptilesetRoot, std::vector<tileset> * ptileset);
		void parse_layer(TiXmlElement * playerRoot, std::vector<game_layer*> *pLayers, const std::vector<tileset> *pTileset);

		int _tilesize;
		int _width;
		int _height;

	private:
		friend class level_parse;
		level_parse();
		
	};
}
#endif //_MIMO_LEVEL_PARSE_