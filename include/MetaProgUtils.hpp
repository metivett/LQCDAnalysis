/*
 * MetaProgUtils.hpp
 *
 *  Created on: Dec 12, 2013
 *      Author: Thibaut Metivet
 */

#include <utility>

namespace LQCDA {
	
template<size_t...>
struct int_seq {};

template<size_t min, typename IntSeq, size_t max>
struct make_int_seq_hlp;
template<size_t min, size_t... s, size_t max>
struct make_int_seq_hlp<min, int_seq<s...>, max>
{
	typedef typename make_int_seq_hlp<min+1, int_seq<s..., min>, max>::type type;
};
template<size_t max, size_t... s>
struct make_int_seq_hlp<max, int_seq<s...>, max> {
	typedef int_seq<s...> type;
};

template<size_t min, size_t max>
struct make_int_seq
{
	std::static_assert(min <= max, "make_int_seq range error");
	typedef typename make_int_seq_hlp<min, int_seq<>, max>::type type;
};

template<typename F, typename Tuple, size_t... s>
auto apply_hlp(F&& f, Tuple&& args, int_seq<s...>)
-> decltype(std::forward<F>(f)(std::get<s>(std::forward<Tuple>(args))...))
	{
		return std::forward<F>(f)(std::get<s>(std::forward<Tuple>(args))...);
	}
template<typename F, typename Tuple>
auto apply_(F&& f, Tuple&& args) 
-> decltype(apply_hlp(std::forward<F>(f), std::forward<Tuple>(args), 
	typename make_int_seq< 0, tuple_size<typename decay<Tuple>::type>::value >::type()))
	{
		return apply_hlp(std::forward<F>(f), std::forward<Tuple>(args), 
			typename make_int_seq< 0, tuple_size<typename decay<Tuple>::type>::value >::type());
	}



template<size_t I, typename T>
struct pack_element_hlp;

template<typename... T>
struct pack_types {};

template<size_t I>
struct pack_element_hlp<I, pack_types<>>
{
	std::static_assert(I == 0, "pack_element index out of range");
	std::static_assert(I != 0, "pack_element index out of range");
};

template<typename H, typename... T>
struct pack_element_hlp<0, pack_types<H, T...>>
{
	typedef H type;
};

template<size_t I, typename H, typename... T>
struct pack_element_hlp<I, pack_types<H, T...>>
{
	typedef typename pack_element_hlp<I-1, pack_types<T...>>::type type;
};

template<size_t I, typename... T>
struct pack_element
{
	typedef typename pack_element_hlp<I, pack_types<T...>>::type type;
};


template<typename R, size_t I, size_t J, typename... T>
struct Get_hlp
{
	static R& dispatch(T...);
};

template<typename R, size_t I, size_t J, typename H, typename... T>
struct Get_hlp<R, I, J, H, T...>
{
	static R& dispatch(H& h, T&... ts)
	{
		return Get_hlp<R, I, J+1, T...>::dispatch(ts...);
	}
};

template<size_t I, typename H, typename... T>
struct Get_hlp<H, I, I, H, T...>
{
	static H& dispatch(H& h, T&... ts)
	{
		return h;
	}
};

template<size_t I, typename... T>
typename pack_element<I, T...>::type& get(T&... ts)
{
	return Get_hlp<typename pack_element<I, T...>::type, I, 0, T...>::dispatch(ts...);
}


template<size_t I, typename F, typename Arg>
class IndexBind_t
{
private:
	F f_;
	Arg x_;

	template<size_t... s1, size_t... s2, typename... Args>
	double call_f(int_seq<s1...>, int_seq<s2...>, Args&&... a)
	{
		return std::forward<F>(f_)(get<s1>(std::forward<Args>(a)...)..., std::forward<Arg>(x_), get<s2>(std::forward<Args>(a)...)...);
	}

public:
	Bind(F&& f, Arg&& x): f_(f), x_(x) {}

	template<typename... Args>
	double operator()(Args&&... a)
	{
		return call_f(typename make_int_seq<0, I-1>::type(), typename make_int_seq<I-1, sizeof...(Args)>::type(), a...); 
	}
};

template<size_t I, typename F, typename Arg>
IndexBind_t<I, F, Arg> index_bind(F&& f, Arg&& x) {
	return IndexBind_t<I, F, Arg>(std::forward<F>(f), std::forward<Arg>(x));
}

}