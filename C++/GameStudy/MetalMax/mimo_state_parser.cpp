#include "mimo_state_parser.h"
#include "mimo_debug.h"
#include "mimo_texture_manager.h"
#include "mimo_game.h"
#include "mimo_game_object_factory.h"

namespace mimo{

	state_parser * state_parser::_instance = 0;

	state_parser * state_parser::instance()
	{
		if (_instance == 0){
			_instance = new state_parser();
		}
		return _instance;
	}

	bool state_parser::parse_state(const char * filename, std::string state_id,
		std::vector<game_object*> * pobjects/*,
		std::vector<std::string> * ptexture_ids*/)
	{
		TiXmlDocument doc;

		if (!doc.LoadFile(filename)){
			MIMO_DEBUG(doc.ErrorDesc());
			return false;
		}
		// Get the root element
		TiXmlElement * proot = doc.RootElement();

		TiXmlElement * pStateRoot = 0;

		for (TiXmlElement * e = proot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
			if (std::string(state_id) == e->Value()){// WARNING:e->value is a type of const char *
				// we need cast state_id to std::string to compare them
				pStateRoot = e;
				break;// have found the state
			}
		}

		TiXmlElement * pTextureRoot = 0;
		for (TiXmlElement * e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
			if (std::string("TEXTURES") == e->Value()){
				pTextureRoot = e;
				break;
			}
		}
		parse_textures(pTextureRoot/*, ptexture_ids*/);

		TiXmlElement * pObjectRoot = 0;
		for (TiXmlElement * e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
			if (std::string("OBJECTS") == e->Value()){
				pObjectRoot = e;
				break;
			}
		}
		parse_objects(pObjectRoot, pobjects);

		return true;
	}

	void state_parser::parse_objects(TiXmlElement * pstate_root, std::vector<game_object*> * pobjects)
	{
		assert(pstate_root != 0);

		for (TiXmlElement * e = pstate_root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
			int x, y;
			int w, h;
			int num_frames;
			std::string id;

			e->Attribute("x", &x);
			e->Attribute("y", &y);
			e->Attribute("width", &w);
			e->Attribute("height", &h);
			e->Attribute("numframs", &num_frames);
			
			id = e->Attribute("textureID");


			game_object * pgame_object = game_object_factory::instance()->create(e->Attribute("type"));
			loader_params param(x, y, w, h, id, num_frames);
			pgame_object->load(&param);
			pobjects->push_back(pgame_object);
		}		
	}

	void state_parser::parse_textures(TiXmlElement * pstate_root/*, std::vector<std::string> * ptexture_ids*/)
	{		
		assert(pstate_root != 0);

		for (TiXmlElement * e = pstate_root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
			std::string filename = e->Attribute("filename");
			std::string id = e->Attribute("ID");

			texture_manager::instance()->load(filename, id, game::instance()->get_renderer());
		}
	}

}