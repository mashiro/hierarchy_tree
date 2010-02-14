#ifndef MEL_HIERARCHY_TREE_DETAIL_UNWRAP_HPP
#define MEL_HIERARCHY_TREE_DETAIL_UNWRAP_HPP

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace mel {
namespace detail { namespace hierarchy_tree {

/* unwrap */
template <typename T>
struct unwrap
{
	typedef T type;
};

template <typename T>
struct unwrap< boost::reference_wrapper<T> >
{
	typedef T type;
};

template <typename T>
struct unwrap< boost::shared_ptr<T> >
{
	typedef T type;
};

template <typename T>
struct unwrap< boost::weak_ptr<T> >
{
	typedef T type;
};

/* unwrap_ref */
template <typename T>
inline typename boost::unwrap_reference<T>::type& unwrap_ref(T& t)
{
	return t;
}

/* unwrap_function */
template <typename Func>
class unwrap_function
{
public:
	typedef void result_type;

	unwrap_function(Func func)
		: func_(func)
	{}

	template <typename T>
	void operator ()(T& arg)
	{
		namespace here = mel::detail::hierarchy_tree;
		return here::unwrap_ref(func_)(arg);
	}

	template <typename T>
	void operator ()(const T& arg) const
	{
		namespace here = mel::detail::hierarchy_tree;
		return here::unwrap_ref(func_)(arg);
	}

private:
	Func func_;
};

template <typename Func>
inline unwrap_function<Func> make_unwrap_function(Func func)
{
	return unwrap_function<Func>(func);
}

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_UNWRAP_HPP
