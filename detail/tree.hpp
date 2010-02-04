#ifndef MEL_HIERARCHY_TREE_DETAIL_TREE_HPP
#define MEL_HIERARCHY_TREE_DETAIL_TREE_HPP

#include "cons.hpp"
#include "unwrap.hpp"
#include <boost/call_traits.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>
#include <typeinfo>

namespace mel {
namespace detail { namespace hierarchy_tree {

namespace here = mel::detail::hierarchy_tree;

//* tree */
template <typename T, typename SiblingPolicy, typename ChildrenPolicy>
class tree
{
public:
	typedef tree<T, SiblingPolicy, ChildrenPolicy> tree_type;
	typedef typename boost::call_traits<T>::value_type value_type;
	typedef typename boost::call_traits<T>::param_type param_type;
	typedef typename boost::mpl::apply<SiblingPolicy, T>::type sibling_type;
	typedef typename boost::mpl::apply<ChildrenPolicy, const std::type_info*, tree_type>::type children_type;

public:
	/* insert */
	template <typename Cons>
	void insert(param_type value)
	{
		_find<Cons>().sibling_.push_back(value);
	}

	/* for_each */
	template <typename Cons, typename Func>
	void for_each(Func func, bool recursive)
	{
		_find<Cons>()._for_each(func, recursive);
	}

	template <typename Cons, typename Func>
	void for_each(Func func, bool recursive) const
	{
		_find<Cons>()._for_each(func, recursive);
	}

	/* for_each_extended */
	template <typename Cons, typename Func>
	void for_each_extended(Func func, bool recursive)
	{
		_find<Cons>()._for_each_extended(func, recursive);
	}

	template <typename Cons, typename Func>
	void for_each_extended(Func func, bool recursive) const
	{
		_find<Cons>()._for_each_extended(func, recursive);
	}

private:
	/* _find */
	template <typename Cons>
	tree_type& _find()
	{
		return _find_impl<Cons, is_tail<Cons> >();
	}

	template <typename Nil, typename IsTail>
	tree_type& _find_impl(typename boost::enable_if<IsTail>::type* = 0)
	{
		return *this;
	}

	template <typename Cons, typename IsTail>
	tree_type& _find_impl(typename boost::disable_if<IsTail>::type* = 0)
	{
		const std::type_info* id = &typeid(typename Cons::head_type);
		return _get_child(id)._find_impl<typename Cons::tail_type, is_tail<Cons> >();
	}

	template <typename Cons>
	const tree_type& _find() const
	{
		return _find_impl<Cons, is_tail<Cons> >();
	}

	template <typename Nil, typename IsTail>
	const tree_type& _find_impl(typename boost::enable_if<IsTail>::type* = 0) const
	{
		return *this;
	}

	template <typename Cons, typename IsTail>
	const tree_type& _find_impl(typename boost::disable_if<IsTail>::type* = 0) const
	{
		const std::type_info* id = &typeid(typename Cons::head_type);
		return _get_child(id)._find_impl<typename Cons::tail_type, is_tail<Cons> >();
	}

	/* _get_child */
	tree_type& _get_child(const std::type_info* id)
	{
		return children_[id];			
	}

	const tree_type& _get_child(const std::type_info* id) const
	{
		return static_cast<const tree_type&>(const_cast<children_type&>(children_)[id]);
	}

	/* _for_each */
	template <typename Func>
	void _for_each(Func func, bool recursive)
	{
		here::unwrap_ref(func)(sibling_);

		if (recursive)
		{
			for (typename children_type::iterator it = children_.begin(); it != children_.end(); ++it)
				it->second._for_each(func, recursive);
		}
	}

	template <typename Func>
	void _for_each(Func func, bool recursive) const
	{
		here::unwrap_ref(func)(sibling_);

		if (recursive)
		{
			for (typename children_type::const_iterator it = children_.begin(); it != children_.end(); ++it)
				it->second._for_each(func, recursive);
		}
	}

	/* _for_each_extended */
	template <typename Func>
	void _for_each_extended(Func func, bool recursive)
	{
		using here::unwrap_ref;
		for (typename sibling_type::iterator it = sibling_.begin(); it != sibling_.end(); ++it)
			here::unwrap_ref(func)(*it);

		if (recursive)
		{
			for (typename children_type::iterator it = children_.begin(); it != children_.end(); ++it)
				it->second._for_each_extended(func, recursive);
		}
	}

	template <typename Func>
	void _for_each_extended(Func func, bool recursive) const
	{
		for (typename sibling_type::const_iterator it = sibling_.begin(); it != sibling_.end(); ++it)
			here::unwrap_ref(func)(*it);

		if (recursive)
		{
			for (typename children_type::const_iterator it = children_.begin(); it != children_.end(); ++it)
				it->second._for_each_extended(func, recursive);
		}
	}

private:
	sibling_type sibling_;
	children_type children_;
};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_TREE_HPP