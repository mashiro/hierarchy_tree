#ifndef MEL_HIERARCHY_TREE_DETAIL_METADATA_HPP
#define MEL_HIERARCHY_TREE_DETAIL_METADATA_HPP

#include "nil.hpp"
#include "cons.hpp"
#include <boost/mpl/has_xxx.hpp>
#include <boost/preprocessor/cat.hpp>

namespace mel {
namespace detail { namespace hierarchy_tree {

/* metadata */
BOOST_MPL_HAS_XXX_TRAIT_DEF(MEL_HIERARCHY_TREE_METADATA_TYPE_NAME);

template <typename T, bool HasMetadata>
struct metadata_impl
{
	typedef typename append<
		typename T::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME,
		T
	>::type type;
};

template <typename T>
struct metadata_impl<T, false>
{
	typedef cons<
		T,
		nil
	> type;
};

template <typename T>
struct metadata
	: metadata_impl<T, BOOST_PP_CAT(has_, MEL_HIERARCHY_TREE_METADATA_TYPE_NAME)<T>::value>
{};

/* metadata_type */
template <typename T>
struct metadata_type
{
	typedef typename T::MEL_HIERARCHY_TREE_METADATA_TYPE_NAME type;
};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_METADATA_HPP