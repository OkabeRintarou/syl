/**
* @file stl_construct.h
* @brief ��������
* 
* @construct(T1 * p,const T2 & value):
*	��p����������T2�Ĺ��캯��
* @destroy(T * pointer)
*   ����pointer��ָ����
* @destroy(ForwardIterator first,ForwardIterator last)
*   ����ForwardIterator������value_type�Ƿ���trivial_destructor������Ӧ�ĺ���

* @author sylcoder
* @version 1.0
* @date 2014/12/30
*/


#ifndef _STL_CONSTRUCT_INCLUDED_
#define _STL_CONSTRUCT_INCLUDED_

#include <new> // placement new������
#include <Type_Traits.h>

namespace MiniSTL{
	template<typename T1,typename T2>
	inline void construct(T1 * p, const T2 & value){
		::new(static_cast<void*>(p)) T1(value);
	}

	// destroy�汾1������һ��ָ��
	template<typename T>
	inline void destroy(T * pointer){
		pointer->~T();
	}

	// destroy�汾2������һ�Ե�����
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