/**
* @file stl_construct.h
* @brief 对象构造器
* 
* @construct(T1 * p,const T2 & value):
*	在p处调用类型T2的构造函数
* @destroy(T * pointer)
*   析构pointer所指对象
* @destroy(ForwardIterator first,ForwardIterator last)
*   根据ForwardIterator包含的value_type是否含有trivial_destructor调用相应的函数

* @author sylcoder
* @version 1.0
* @date 2014/12/30
*/


#ifndef _STL_CONSTRUCT_INCLUDED_
#define _STL_CONSTRUCT_INCLUDED_

#include <new> // placement new操作符
#include <Type_Traits.h>

namespace MiniSTL{
	template<typename T1,typename T2>
	inline void construct(T1 * p, const T2 & value){
		::new(static_cast<void*>(p)) T1(value);
	}

	// destroy版本1：接受一个指针
	template<typename T>
	inline void destroy(T * pointer){
		pointer->~T();
	}

	// destroy版本2：接受一对迭代器
	template<typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		__destroy(first, last, value_type(first));
	}

	template<typename ForwardIterator,typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
	{
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for (; first != last, ++first){
			destroy(&*first);
		}
	}

	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
	{

	}

	inline void destroy(char *, char *){}
	inline void destroy(wchar_t *, wchar_t *){}

}

#endif