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
typename boost::unwrap_reference<T>::type& unwrap_ref(T& t)
{
	return t;
}

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_UNWRAP_HPP