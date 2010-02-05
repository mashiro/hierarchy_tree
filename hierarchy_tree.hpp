#ifndef MEL_HIERARCHY_TREE_HIERARCHY_TREE_HPP
#define MEL_HIERARCHY_TREE_HIERARCHY_TREE_HPP

#ifndef MEL_HIERARCHY_TREE_METADATA_TYPE_NAME
#define MEL_HIERARCHY_TREE_METADATA_TYPE_NAME hierarchy_tree_metadata_type
#endif // MEL_HIERARCHY_TREE_METADATA_TYPE_NAME

#include "detail/metadata.hpp"
#include "detail/unwrap.hpp"
#include "detail/policy.hpp"
#include "detail/tree.hpp"

#define MEL_HIERARCHY_TREE_METADATA_DEF(name) \
	typedef mel::detail::hierarchy_tree::metadata<name>::type MEL_HIERARCHY_TREE_METADATA_TYPE_NAME \
/**/

namespace mel {

/* hierarchy tree */
template <
	typename T,
	typename SiblingPolicy = detail::hierarchy_tree::default_sibling_policy,
	typename ChildrenPolicy = detail::hierarchy_tree::default_children_policy
>
class hierarchy_tree
{
public:
	typedef detail::hierarchy_tree::tree<T, SiblingPolicy, ChildrenPolicy> tree_type;
	typedef typename tree_type::value_type value_type;
	typedef typename tree_type::sibling_type sibling_type;
	typedef typename tree_type::children_type children_type;

public:
	/* insert */
	template <typename U>
	void insert(const U& value)
	{
		typedef typename detail::hierarchy_tree::unwrap<U>::type unwrapped;
		insert<U, unwrapped>(value);
	}

	template <typename U, typename Unwrapped>
	void insert(const U& value)
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.template insert<metadata>(value);
	}

	/* for_each */
	template <typename Func>
	void for_each(Func func, bool recursive = true)
	{
		typedef typename detail::hierarchy_tree::unwrap<T>::type unwrapped;
		for_each<unwrapped>(func, recursive);
	}

	template <typename Func>
	void for_each(Func func, bool recursive = true) const
	{
		typedef typename detail::hierarchy_tree::unwrap<T>::type unwrapped;
		for_each<unwrapped>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each(Func func, bool recursive = true)
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.template for_each<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.template for_each<metadata>(func, recursive);
	}

	/* for_each_extended */
	template <typename Func>
	void for_each_extended(Func func, bool recursive = true)
	{
		typedef typename detail::hierarchy_tree::unwrap<T>::type unwrapped;
		for_each_extended<unwrapped>(func, recursive);
	}

	template <typename Func>
	void for_each_extended(Func func, bool recursive = true) const
	{
		typedef typename detail::hierarchy_tree::unwrap<T>::type unwrapped;
		for_each_extended<unwrapped>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each_extended(Func func, bool recursive = true)
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.template for_each_extended<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each_extended(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.template for_each_extended<metadata>(func, recursive);
	}

private:
	tree_type root_;
};

} // namespace mel

#endif // MEL_HIERARCHY_TREE_HIERARCHY_TREE_HPP
