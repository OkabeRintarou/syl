#ifndef _MIMO_OBJECT_
#define _MIMO_OBJECT_

#include "mimo_include.h"
#include "SDL.h"

namespace mimo{
	class loader_params
	{
	public:
		loader_params() :_x(0), _y(0), _width(0), _height(0){}
		loader_params(int x, int y, int width, int height, std::string texture_id,int num_frames) :
			_x(x), _y(y), _width(width), _height(height), _texture_id(texture_id),_num_frams(num_frames){}
		int get_x()const{ return _x; }
		int get_y()const{ return _y; }
		int get_width()const{ return _width; }
		int get_height()const{ return _height; }
		std::string get_id()const{ return _texture_id; }
		int get_num_frames()const{ return _num_frams; }

		void set_x(int x){_x = x;}
		void set_y(int y){ _y = y; }
		void set_w(int w){_width = w;}
		void set_h(int h){ _height = h; }
		void set_id(std::string id){ _texture_id = id; }
	private:
		int _x;
		int _y;

		int _width;
		int _height;

		std::string _texture_id;
		int _num_frams;
	};

	class game_object
	{
	public:
		virtual void draw(void) = 0;
		virtual void update(void) = 0;
		virtual void clean(void) = 0;

		virtual void load(const loader_params * params) = 0;

	protected:
		game_object();
		virtual ~game_object(){}

		std::string _texture_id;

	
	};
}
#endif //_MIMO_OBJECT_