/**
* @file stl_uninitialized.h
* @brief 内存基本处理工具
*
* @author sylcoder
* @version 1.0
* @date 2015/01/01
* @TODO:line 78 copy算法的实现
*/

#ifndef _STL_UNINITIALIZED_INCLUDED_
#define _STL_UNINITIALIZED_INCLUDED_

#include <stl_construct.h>
#include <stl_algobase.h>
#include <Type_Traits.h>
#include <Iteration_Traits.h>


namespace MiniSTL{

	template<typename ForwardIterator,typename Size,typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
		Size n, const T & x){
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

	template<typename ForwardIterator,typename Size,typename T,typename T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
		Size n,const T & value, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, value, is_POD());
	}

	template<typename ForwardIterator,typename Size,typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T & value, __true_type)
	{
		return fill_n(first, n, value);
	}

	template<typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T & value, __false_type)
	{
		ForwardIterator cur = first;
		try{
			for (; n > 0; --n, ++cur){
				construct(&*cur, value);
			}
		}
		catch (...){
			destroy(first, cur);
			throw;
		}
		return cur;
	}

	template<typename InputIterator,typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, 
		InputIterator last, ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	template<typename InputIterator,typename ForwardIterator,typename T>
	inline ForwardIterator __uninitialized_copy(InputIterator first,
		InputIterator last, ForwardIterator result, T*)
	{
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	template<typename InputIterator,typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, __true_type){
		// TODO:stl::copy(first,last,result)
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur){
			*cur = *first;
		}
		return cur;
	}

	template<typename InputIterator,typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, __false_type){

		ForwardIterator cur = result;

		try{
			for (; first != last; ++first, ++cur){
				construct(&*cur, *first);
			}
		}
		catch(...){
			destroy(first, cur);
		}
		return cur;
	}

	inline char * uninitialized_copy(char * first, char * last, char * result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t * uninitialized_copy(wchar_t * first, wchar_t * last, wchar_t * result)
	{
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

}

#endif