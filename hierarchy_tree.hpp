#ifndef MEL_HIERARCHY_TREE_HPP
#define MEL_HIERARCHY_TREE_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/call_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/cast.hpp>
#include <typeinfo>
#include <list>
#include <map>

#define MEL_HIERARCHY_TREE_METADATA_DEF(name) \
	typedef mel::detail::hierarchy_tree::metadata<name>::type hierarchy_tree_metadata_type \
/**/

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

	/* metadata */
	BOOST_MPL_HAS_XXX_TRAIT_DEF(hierarchy_tree_metadata_type);

	template <typename T, bool C>
	struct metadata_impl
	{
		typedef typename append<
			typename T::hierarchy_tree_metadata_type,
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
		: metadata_impl<T, has_hierarchy_tree_metadata_type<T>::value>
	{};

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

	/* tree */
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
			unwrap_ref(func)(sibling_);

			if (recursive)
			{
				for (typename children_type::iterator it = children_.begin(); it != children_.end(); ++it)
					it->second._for_each(func, recursive);
			}
		}

		template <typename Func>
		void _for_each(Func func, bool recursive) const
		{
			unwrap_ref(func)(sibling_);

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
			for (typename sibling_type::iterator it = sibling_.begin(); it != sibling_.end(); ++it)
				unwrap_ref(func)(*it);

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
				unwrap_ref(func)(*it);

			if (recursive)
			{
				for (typename children_type::const_iterator it = children_.begin(); it != children_.end(); ++it)
					it->second._for_each_extended(func, recursive);
			}
		}

		/* unwrap_ref */
		template <typename T>
		typename boost::unwrap_reference<T>::type& unwrap_ref(T& t) const
		{
			return t;
		}

	private:
		sibling_type sibling_;
		children_type children_;
	};

}} // namespace detail::hierarchy_tree

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
		typedef typename Unwrapped::hierarchy_tree_metadata_type metadata;
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
		typedef typename Unwrapped::hierarchy_tree_metadata_type metadata;
		root_.for_each<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::hierarchy_tree_metadata_type metadata;
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
		typedef typename Unwrapped::hierarchy_tree_metadata_type metadata;
		root_.for_each_extended<metadata>(func, recursive);
	}

	template <typename Unwrapped, typename Func>
	void for_each_extended(Func func, bool recursive = true) const
	{
		typedef typename Unwrapped::hierarchy_tree_metadata_type metadata;
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
