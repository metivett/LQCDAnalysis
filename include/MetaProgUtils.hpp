/*
 * MetaProgUtils.hpp
 *
 *  Created on: Dec 12, 2013
 *      Author: Thibaut Metivet
 */

#ifndef META_PROG_UTILS_HPP
#define META_PROG_UTILS_HPP

#include <utility>

namespace LQCDA {
	
template<std::size_t...>
struct int_seq {};

template<std::size_t min, typename IntSeq, std::size_t max>
struct make_int_seq_hlp;
template<std::size_t min, std::size_t... s, std::size_t max>
struct make_int_seq_hlp<min, int_seq<s...>, max>
{
	typedef typename make_int_seq_hlp<min+1, int_seq<s..., min>, max>::type type;
};
template<std::size_t max, std::size_t... s>
struct make_int_seq_hlp<max, int_seq<s...>, max> {
	typedef int_seq<s...> type;
};

template<std::size_t min, std::size_t max>
struct make_int_seq
{
	static_assert(min <= max, "make_int_seq range error");
	typedef typename make_int_seq_hlp<min, int_seq<>, max>::type type;
};

template<typename F, typename Tuple, std::size_t... s>
auto apply_hlp(F&& f, Tuple&& args, int_seq<s...>)
-> decltype(std::forward<F>(f)(std::get<s>(std::forward<Tuple>(args))...))
	{
		return std::forward<F>(f)(std::get<s>(std::forward<Tuple>(args))...);
	}
template<typename F, typename Tuple>
auto apply_(F&& f, Tuple&& args) 
-> decltype(apply_hlp(std::forward<F>(f), std::forward<Tuple>(args), 
	typename make_int_seq< 0, std::tuple_size<typename std::decay<Tuple>::type>::value >::type()))
	{
		return apply_hlp(std::forward<F>(f), std::forward<Tuple>(args), 
			typename make_int_seq< 0, std::tuple_size<typename std::decay<Tuple>::type>::value >::type());
	}



template<std::size_t I, typename T>
struct pack_element_hlp;

template<typename... T>
struct pack_types {};

template<std::size_t I>
struct pack_element_hlp<I, pack_types<>>
{
	static_assert(I == 0, "pack_element index out of range");
	static_assert(I != 0, "pack_element index out of range");
};

template<typename H, typename... T>
struct pack_element_hlp<0, pack_types<H, T...>>
{
	typedef H type;
};

template<std::size_t I, typename H, typename... T>
struct pack_element_hlp<I, pack_types<H, T...>>
{
	typedef typename pack_element_hlp<I-1, pack_types<T...>>::type type;
};

template<std::size_t I, typename... T>
struct pack_element
{
	typedef typename pack_element_hlp<I, pack_types<T...>>::type type;
};


template<typename R, std::size_t I, std::size_t J, typename... T>
struct Get_hlp
{
	static R& dispatch(T...);
};

template<typename R, std::size_t I, std::size_t J, typename H, typename... T>
struct Get_hlp<R, I, J, H, T...>
{
	static R& dispatch(H& h, T&... ts)
	{
		return Get_hlp<R, I, J+1, T...>::dispatch(ts...);
	}
};

template<std::size_t I, typename H, typename... T>
struct Get_hlp<H, I, I, H, T...>
{
	static H& dispatch(H& h, T&... ts)
	{
		return h;
	}
};

template<std::size_t I, typename... T>
typename pack_element<I, T...>::type& get(T&... ts)
{
	return Get_hlp<typename pack_element<I, T...>::type, I, 0, T...>::dispatch(ts...);
}


template<std::size_t I, typename F, typename Arg>
class IndexBind_t
{
private:
	F f_;
	Arg x_;

	template<std::size_t... s1, std::size_t... s2, typename... Args>
	double call_f(int_seq<s1...>, int_seq<s2...>, Args&&... a)
	{
		return std::forward<F>(f_)(get<s1>(std::forward<Args>(a)...)..., std::forward<Arg>(x_), get<s2>(std::forward<Args>(a)...)...);
	}

public:
	IndexBind_t(F&& f, Arg&& x): f_(f), x_(x) {}

	template<typename... Args>
	double operator()(Args&&... a)
	{
		return call_f(typename make_int_seq<0, I-1>::type(), typename make_int_seq<I-1, sizeof...(Args)>::type(), a...); 
	}
};

template<std::size_t I, typename F, typename Arg>
IndexBind_t<I, F, Arg> index_bind(F&& f, Arg&& x) {
	return IndexBind_t<I, F, Arg>(std::forward<F>(f), std::forward<Arg>(x));
}

}

#endif // META_PROG_UTILS_HPP