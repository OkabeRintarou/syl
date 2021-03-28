//#include "mimo_level_parse.h"
//#include "mimo_texture_manager.h"
//#include "mimo_include.h"
//#include "mimo_game.h"
//
//namespace mimo{
//	game_level * level_parse::parse_level(const char * filename)
//	{
//		// Load the map XML
//		TiXmlDocument doc;
//		int ret = doc.LoadFile(filename);
//		assert(ret != false);
//
//		// Create the level object
//		game_level * level = new game_level();
//
//		TiXmlElement * root = doc.RootElement();
//		
//		root->Attribute("tilewidth", &_tilesize);
//		root->Attribute("width", &_width);
//		root->Attribute("height",&_height);
//
//		// parse the tilesets
//		for (TiXmlElement * e = root->FirstChildElement();
//			e != NULL;
//			e = e->NextSiblingElement()){
//			if (e->Value() == std::string("tileset")){
//				parse_tileset(e, level->get_tilesets());
//				break;
//			}
//		}
//
//		// parse any object layer
//		for (TiXmlElement * e = root->FirstChildElement();
//			e != NULL;
//			e = e->NextSiblingElement()){
//			if (e->Value() == std::string("layer")){
//				parse_layer(e, level->get_layers(), level->get_tilesets());
//				break;
//			}
//		}
//
//		return level;
//	}
//
//	void level_parse::parse_tileset(TiXmlElement * ptilesetRoot, std::vector<tileset> * ptileset)
//	{
//		// first add the tileset to texture manager
//		texture_manager::instance()->load(ptilesetRoot->FirstChildElement()->Attribute("source"),
//			ptilesetRoot->Attribute("name"), game::instance()->get_renderer());
//
//	}
//}