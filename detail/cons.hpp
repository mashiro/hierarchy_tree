#ifndef MEL_HIERARCHY_TREE_DETAIL_CONS_HPP
#define MEL_HIERARCHY_TREE_DETAIL_CONS_HPP

#include "nil.hpp"
#include <boost/mpl/bool.hpp>

namespace mel {
namespace detail { namespace hierarchy_tree {

/* cons */
template <typename Head, typename Tail>
struct cons
{
	typedef Head head_type;
	typedef Tail tail_type;
};

template <typename List>
struct is_tail
	: boost::mpl::false_
{};

template <typename Head>
struct is_tail< cons<Head, nil> >
	: boost::mpl::true_
{};

/* append */
template <typename List, typename T>
struct append;

template <>
struct append<nil, nil>
{
	typedef nil type;
};

template <typename T>
struct append<nil, T>
{
	typedef cons<T, nil> type;
};

template <typename Head, typename Tail>
struct append<nil, cons<Head, Tail> >
{
	typedef cons<Head, Tail> type;
};

template <typename Head, typename Tail, typename T>
struct append<cons<Head, Tail>, T>
{
	typedef cons<Head, typename append<Tail, T>::type> type;
};

}} // namespace detail::hierarchy_tree
} // namespace mel

#endif // MEL_HIERARCHY_TREE_DETAIL_CONS_HPP