#ifndef _MIMO_TEST_
#define _MIMO_TEST

#include "mimo_include.h"
#include "mimo_sdl_game_object.h"
#include "mimo_game_state.h"
#include "mimo_game_object_factory.h"

namespace mimo{
	class menu_button:public sdl_game_object
	{
	public:
		enum button_state
		{
			MOUSE_OUT = 0,
			MOUSE_OVER = 1,
			CLICKED = 2
		};
	public:
		menu_button();

		virtual void update(void);
		virtual void draw(void);
		virtual void clean(void);
		void load(const loader_params *);
	};

	class menu_creator :public base_creator
	{
	public:
		game_object * create_game_object()const
		{
			return new menu_button();
		}
	};

	// Player
 
	class player :public sdl_game_object
	{
		virtual void update(void)
		{

		}
		virtual void draw(void)
		{

		}
		virtual void clean(void)
		{

		}
		void load(const loader_params *)
		{

		}
	};

	//
	class menu_state :public game_state
	{
	public:
		menu_state() :game_state(){}
		virtual void update(void);
		virtual void render(void);
		virtual void enter(void);
		virtual bool quit(void);

		virtual std::string get_id()const{
			return _menu_state_id;
		}
	private:
		static const std::string _menu_state_id;
		std::vector<game_object*> _game_objects;
	};

}
#endif //_MIMO_TEST