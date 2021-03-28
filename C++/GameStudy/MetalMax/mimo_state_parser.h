#ifndef _MIMO_STATE_PARSER_
#define _MIMO_STATE_PARSER_

#include "mimo_include.h"
#include "mimo_sdl_game_object.h"
#include "tinyxml.h"

namespace mimo{
	class state_parser
	{
	public:
		static state_parser * instance();
		bool parse_state(const char * filename, std::string state_id,
			std::vector<game_object*> * pobjects/*,
			std::vector<std::string> * ptexture_ids*/);
	private:
		void parse_objects(TiXmlElement * pstate_root, std::vector<game_object*> * pobjects);
		void parse_textures(TiXmlElement * pstate_root/*, std::vector<std::string> * ptexture_ids*/);

		static state_parser * _instance;
	};
}
#endif //_MIMO_STATE_PARSER_