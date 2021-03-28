/**
* @file Vector.h
* @brief 自动内存增长序列容器
*
* 本示例文件中包含了类vector的声明
*
* @author sylcoder
* @version 1.0
* @date 2014/12/26
*/

#ifndef _VECTOR_H_INCLUDED_
#define _VECTOR_H_INCLUDED_

namespace MiniSTL{
	template<typename T,typename Alloc>
	class _vector_base{
	public:
		typedef Alloc allocator_type;

	protected:
		T * _start;
		T * _last;
		T * _end_of_storage;
	};

	template<typename T,typename Alloc = allocator>
	class vector:protected _vector_base<T,Alloc>
	{
	public:
		typedef Alloc allocator_type;

		typedef T value_type;
		typedef T * pointer;
		typedef const T * pointer;
		typedef T & reference;
		typedef const T & const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T * iterator;
		typedef T const * const_iterator;

	public:
		iterator begin(){ return _start; }
		const_iterator begin()const{ return _start; }
		iterator end(){ return _last; }
		const_iterator end()const{ return _last; }


		size_type size()const{ return size_type(end() - begin()); }
		size_type max_size()const{ return size_type(-1) / sizeof(T); }//vector容器最多能容纳元素T的个数
		bool empty()const{ return (end() == begin()); }
		size_type capacity()const{ return _end_of_storage - begin(); }
	};
}
#endif
