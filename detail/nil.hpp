#ifndef MEL_HIERARCHY_TREE_DETAIL_NIL_HPP
#define MEL_HIERARCHY_TREE_DETAIL_NIL_HPP

#include <boost/mpl/bool.hpp>

namespace mel {
namespace detail { namespace hierarchy_tree {

/* nil */
struct nil {};

template <typename T>
struct is_nil
	: boost::mpl::false_
{};

template <>
struct is_nil<nil>
	: boost::mpl::true_
{};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_NIL_HPP