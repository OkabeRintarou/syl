
#ifndef _ITERATION_TRAITS_INCLUDED_
#define _ITERATION_TRAITS_INCLUDED_

namespace MiniSTL{

	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag :public input_iterator_tag{};
	struct bidirectional_iterator_tag :public forward_iterator_tag{};
	struct random_access_iterator_tag :public bidirectional_iterator_tag{};

	template<typename Category,
		typename T,
		typename Diff = ptrdiff_t,
		typename Pointer = T *,
		typename Reference = T&>
	struct iterator{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Diff		difference_type;
		typedef Pointer     pointer;
		typedef Reference   reference;
	};

	template<typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	template<typename T>
	struct iterator_traits<T*>
	{
		typedef T value_type;
		typedef T & reference;
		typedef T * pointer;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	struct iterator_traits<T const *>
	{
		typedef T value_type;
		typedef T & reference;
		typedef T const * pointer;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	struct iterator_category<T * const>
	{
		typedef T value_type;
		typedef T & reference;
		typedef T  * const pointer;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename Iterator>
	inline typename Iterator::iterator_category
		iterator_category(const Iterator &){
			typedef typename Iterator::iterator_category category;
			return category();
		}

	template<typename Iterator>
	inline typename Iterator::value_type*
		value_type(const Iterator &){
			return static_cast <Iterator::value_type*>(0);
		}


}
#endif 