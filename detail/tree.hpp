#ifndef MEL_HIERARCHY_TREE_DETAIL_TREE_HPP
#define MEL_HIERARCHY_TREE_DETAIL_TREE_HPP

#include "cons.hpp"
#include "unwrap.hpp"
#include <boost/call_traits.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <typeinfo>

namespace mel {
namespace detail { namespace hierarchy_tree {

//* tree */
template <typename T, typename SiblingPolicy, typename ChildrenPolicy>
class tree
{
public:
	typedef tree<T, SiblingPolicy, ChildrenPolicy> self;
	typedef self tree_type;
	typedef typename boost::call_traits<T>::value_type value_type;
	typedef typename boost::call_traits<T>::param_type param_type;
	typedef typename boost::mpl::apply<SiblingPolicy, T>::type sibling_type;
	typedef typename boost::mpl::apply<ChildrenPolicy, const std::type_info*, tree_type>::type children_type;

public:
	/* insert */
	template <typename Cons>
	void insert(param_type value)
	{
		find_node<Cons>().sibling_.push_back(value);
	}

	/* for_each */
	template <typename Cons, typename Func>
	void for_each(Func func, bool recursive)
	{
		find_node<Cons>().for_each_impl(func, recursive);
	}

	template <typename Cons, typename Func>
	void for_each(Func func, bool recursive) const
	{
		find_node<Cons>().for_each_impl(func, recursive);
	}

	/* for_each_extended */
	template <typename Cons, typename Func>
	void for_each_extended(Func func, bool recursive)
	{
		find_node<Cons>().for_each_extended_impl(func, recursive);
	}

	template <typename Cons, typename Func>
	void for_each_extended(Func func, bool recursive) const
	{
		find_node<Cons>().for_each_extended_impl(func, recursive);
	}

private:
	/* get_child */
	tree_type& get_child(const std::type_info* id)
	{
		return children_[id];			
	}

	const tree_type& get_child(const std::type_info* id) const
	{
		return static_cast<const tree_type&>(const_cast<children_type&>(children_)[id]);
	}

	/* find_node */
	template <typename Cons>
	tree_type& find_node()
	{
		return find_node_impl<Cons, is_tail<Cons> >();
	}

	template <typename Nil, typename IsTail>
	tree_type& find_node_impl(typename boost::enable_if<IsTail>::type* = 0)
	{
		return *this;
	}

	template <typename Cons, typename IsTail>
	tree_type& find_node_impl(typename boost::disable_if<IsTail>::type* = 0)
	{
		const std::type_info* id = &typeid(typename Cons::head_type);
		return get_child(id).template find_node_impl<typename Cons::tail_type, is_tail<Cons> >();
	}

	template <typename Cons>
	const tree_type& find_node() const
	{
		return find_node_impl<Cons, is_tail<Cons> >();
	}

	template <typename Nil, typename IsTail>
	const tree_type& find_node_impl(typename boost::enable_if<IsTail>::type* = 0) const
	{
		return *this;
	}

	template <typename Cons, typename IsTail>
	const tree_type& find_node_impl(typename boost::disable_if<IsTail>::type* = 0) const
	{
		const std::type_info* id = &typeid(typename Cons::head_type);
		return get_child(id).template find_node_impl<typename Cons::tail_type, is_tail<Cons> >();
	}

	/* for_each_impl */
	template <typename Func>
	void for_each_impl(Func func, bool recursive)
	{
		make_unwrap_function(func)(sibling_);

		if (recursive)
		{
			for (typename children_type::iterator it = children_.begin(); it != children_.end(); ++it)
				it->second.for_each_impl(func, recursive);
		}
	}

	template <typename Func>
	void for_each_impl(Func func, bool recursive) const
	{
		make_unwrap_function(func)(sibling_);

		if (recursive)
		{
			for (typename children_type::const_iterator it = children_.begin(); it != children_.end(); ++it)
				it->second.for_each_impl(func, recursive);
		}
	}

	/* for_each_extended_impl */
	template <typename Func>
	void for_each_extended_impl(Func func, bool recursive)
	{
		std::for_each(sibling_.begin(), sibling_.end(), make_unwrap_function(func));

		if (recursive)
		{
			for (typename children_type::iterator it = children_.begin(); it != children_.end(); ++it)
				it->second.for_each_extended_impl(func, recursive);
		}
	}

	template <typename Func>
	void for_each_extended_impl(Func func, bool recursive) const
	{
		std::for_each(sibling_.begin(), sibling_.end(), make_unwrap_function(func));

		if (recursive)
		{
			for (typename children_type::const_iterator it = children_.begin(); it != children_.end(); ++it)
				it->second.for_each_extended_impl(func, recursive);
		}
	}

private:
	sibling_type sibling_;
	children_type children_;
};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_TREE_HPP
