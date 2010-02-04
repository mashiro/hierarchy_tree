#ifndef MEL_HIERARCHY_TREE_EXTRACTOR_HPP
#define MEL_HIERARCHY_TREE_EXTRACTOR_HPP

#include <boost/cast.hpp>
#include <boost/shared_ptr.hpp>

namespace mel {

/* converter */
template <typename Target>
struct hierarchy_tree_value_converter
{
	typedef Target result_type;

	template <typename Source>
	result_type operator ()(const Source& source) const
	{
		return boost::polymorphic_downcast<result_type>(source);
	}
};

template <typename Target>
struct hierarchy_tree_value_converter< boost::shared_ptr<Target> >
{
	typedef boost::shared_ptr<Target> result_type;

	template <typename Source>
	result_type operator ()(const Source& source) const
	{
		return boost::shared_polymorphic_downcast<Target>(source);
	}
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
		hierarchy_tree_value_converter<typename Container::value_type> converter;
		for (iter_t it = container.begin(); it != container.end(); ++it)
			c_.push_back(converter(*it));
	}
};

template <typename Container>
hierarchy_tree_extractor<Container> make_hierarchy_tree_extractor(Container& c)
{
	return hierarchy_tree_extractor<Container>(c);
};

} // namespace mel

#endif // MEL_HIERARCHY_TREE_EXTRACTOR_HPP
