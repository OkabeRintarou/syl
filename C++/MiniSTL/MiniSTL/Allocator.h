/**
* @file Allocator.h
* @brief 内存配置器
*
* @author sylcoder
* @version 1.0
* @date 2014/12/27
* @TODO:实现一个真正的内存池，现在仅仅是operator new(delete)的包装
*/
#ifndef _ALLOCATOR_H_INCLUDED_
#define _ALLOCATOR_H_INCLUDED_

#include <cstddef>

namespace MiniSTL
{
	template<typename T1,typename T2>
	inline void __construct(T1 * p, T2 const & value){
		new (p) T1(value);
	}

	template<typename T>
	inline void __destroy(T * p){
		p->~T();
	}

	template<typename T>
	class allocator
	{
	public:
		typedef T			value_type;
		typedef T *			pointer;
		typedef const T*	const_pointer;
		typedef T &			reference;
		typedef const T &	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	public:
		static pointer allocate();
		static pointer allocate(size_type n);
		static void deallocate(pointer p, size_type n);
		static void construct(pointer p, const T & x){
			__construct(p, x);
		}
		static void destroy(pointer p){
			__destroy(p);
		}
	};

	template<typename T>
	typename allocator<T>::pointer allocator<T>::allocate()
	{
		return static_cast<pointer>(::operator new(sizeof(T)));
	}

	template<typename T>
	typename allocator<T>::pointer allocator<T>::allocate(
		typename allocator<T>::size_type n){

		typedef allocator<T>::pointer pointer;
		return static_cast<pointer>(::operator new(sizeof(T) * n));
	}

	template<typename T>
	void allocator<T>::deallocate(pointer p, size_type n){
		::operator delete(p);
	}
}



#endif

