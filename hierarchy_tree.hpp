#ifndef MEL_HIERARCHY_TREE_HPP
#define MEL_HIERARCHY_TREE_HPP

#ifndef MEL_HIERARCHY_TREE_METADATA_TYPE_NAME
#define MEL_HIERARCHY_TREE_METADATA_TYPE_NAME hierarchy_tree_metadata_type
#endif // MEL_HIERARCHY_TREE_METADATA_TYPE_NAME

#include "detail/metadata.hpp"
#include "detail/unwrap.hpp"
#include "detail/policy.hpp"
#include "detail/tree.hpp"
#include <boost/cast.hpp>

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
		root_.insert<metadata>(value);
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
		root_.for_each<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.for_each<metadata>(func, recursive);
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
		root_.for_each_extended<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each_extended(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME metadata;
		root_.for_each_extended<metadata>(func, recursive);
	}

private:
	tree_type root_;
};

/* extractor */
template <typename Container>
struct hierarchy_tree_extractor
{
	Container& c_;
	hierarchy_tree_extractor(Container& c) : c_(c) {}

	template <typename T>
	void operator ()(const T& container) const
	{
		typedef typename T::const_iterator iter_t;
		converter<typename Container::value_type> converter;
		for (iter_t it = container.begin(); it != container.end(); ++it)
			c_.push_back(converter(*it));
	}

	template <typename Target>
	struct converter
	{
		typedef Target result_type;

		template <typename Source>
		result_type operator ()(const Source& source) const
		{
			return boost::polymorphic_downcast<result_type>(source);
		}
	};

	template <typename Target>
	struct converter< boost::shared_ptr<Target> >
	{
		typedef boost::shared_ptr<Target> result_type;

		template <typename Source>
		result_type operator ()(const Source& source) const
		{
			return boost::shared_polymorphic_downcast<Target>(source);
		}
	};
};

template <typename Container>
hierarchy_tree_extractor<Container> make_hierarchy_tree_extractor(Container& c)
{
	return hierarchy_tree_extractor<Container>(c);
};

} // namespace mel

#endif // MEL_HIERARCHY_TREE_HPP
