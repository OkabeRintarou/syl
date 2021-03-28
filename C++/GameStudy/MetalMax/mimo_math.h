#ifndef _MIMO_MATH_
#define _MIMO_MATH_

#include <cmath>

namespace mimo{
	class vector2d
	{
	public:
		vector2d() :_x(0.f), _y(0.f){}
		vector2d(float x, float y) :_x(x), _y(y){}

		float get_x()const{ return _x; }
		float get_y()const{ return _y; }

		void set_x(float x)	{ _x = x; }
		void set_y(float y) { _y = y; }
		void set_xy(float x, float y){ _x = x; _y = y; }

		float length()const{
			return sqrt(_x * _x + _y * _y);
		}

		vector2d operator+(vector2d const & v)const{
			return vector2d(_x + v._x, _y + v._y);
		}

		vector2d operator-(vector2d const & v)const{
			return vector2d(_x - v._x, _y - v._y);
		}

		vector2d operator*(float scalar)const{
			return vector2d(scalar * _x, scalar * _y);
		}

		vector2d operator/(float scalar)const{
			return vector2d(scalar / _x, scalar / _y);
		}


		friend vector2d & operator+=(vector2d &v1, vector2d const & v2)
		{
			v1._x += v2._x;
			v1._y += v2._y;
			return v1;
		}

		friend vector2d & operator-=(vector2d &v1, vector2d const & v2)
		{
			v1._x -= v2._x;
			v1._y -= v2._y;
			return v1;
		}

		vector2d & operator*=(float scalar)
		{
			_x *= scalar;
			_y *= scalar;
			return *this;
		}

		vector2d & operator/=(float scalar)
		{
			_x /= scalar;
			_y /= scalar;
			return *this;
		}

		void normalize(void)
		{
			float len = length();
			if (len > 0){
				 (*this) *= (1 / len);
			}
		}
	private:
		float _x;
		float _y;
	};
}

#endif //_MIMO_MATH_