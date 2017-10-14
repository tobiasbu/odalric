
#ifndef _OAI_FUNCTIONTRAITS_H_
#define _OAI_FUNCTIONTRAITS_H_

#include <tuple>

template <typename R, typename ... Args>
struct Method
{
	typedef R(*type)(Args...);
};




// http://en.cppreference.com/w/cpp/language/parameter_pack
template<typename T>
struct function_traits;

/*
template<typename R, typename ...Args>
struct function_traits<R,Args...)>
{
	Method<R, Args> method;

	static const size_t nargs = sizeof...(Args);

	typedef R result_type;

	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};*/

#endif