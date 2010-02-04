#ifndef MEL_HIERARCHY_TREE_DETAIL_POLICY_HPP
#define MEL_HIERARCHY_TREE_DETAIL_POLICY_HPP

#include <boost/mpl/bool.hpp>
#include <list>
#include <map>

namespace mel {
namespace detail { namespace hierarchy_tree {

/* policy */
struct default_sibling_policy
{
	template <typename T>
	struct apply
	{
		typedef std::list<T> type;
	};
};

struct default_children_policy
{
	template <typename K, typename V>
	struct apply
	{
		typedef std::map<K, V> type;
	};
};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_POLICY_HPP