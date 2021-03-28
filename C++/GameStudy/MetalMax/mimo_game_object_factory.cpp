#include "mimo_game_object_factory.h"
#include "mimo_debug.h"

namespace mimo{

	game_object_factory * game_object_factory::_instance = 0;

	game_object_factory * game_object_factory::instance()
	{
		if (_instance == 0){
			_instance = new game_object_factory();
		}
		return _instance;
	}
	bool game_object_factory::register_type(std::string type_id, base_creator * pcreator)
	{
		std::map<std::string, base_creator*>::iterator itr = _creators.find(type_id);
		if (itr != _creators.end()){
			delete pcreator;
			return false;
		}

		_creators[type_id] = pcreator;
		return true;
	}

	game_object * game_object_factory::create(std::string type_id)
	{
		std::map<std::string, base_creator*>::iterator itr = _creators.find(type_id);
		if (itr == _creators.end()){
			std::string  str = "Have not register creator for £º" + type_id + "\n";
			MIMO_DEBUG(str.c_str());

			return 0;
		}

		base_creator * creator = itr->second;
		return creator->create_game_object();
	}
}