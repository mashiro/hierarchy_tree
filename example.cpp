#include "hierarchy_tree.hpp"
#include "extractor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

class Animal
{
public:
	MEL_HIERARCHY_TREE_METADATA_DEF(Animal);
	virtual std::string name() const { return "Animal"; }
};

#define ANIMAL_DEF(_name, _base) \
	class _name; \
	typedef boost::shared_ptr<_name> _name##Ptr; \
	class _name : public _base \
	{ \
	public: \
		MEL_HIERARCHY_TREE_METADATA_DEF(_name); \
		virtual std::string name() const { return _base::name() + " -> " + #_name; } \
		static _name##Ptr create() { return _name##Ptr(new _name()); } \
	}; \
/**/

ANIMAL_DEF(Dog, Animal);
ANIMAL_DEF(BorderCollie, Dog);
ANIMAL_DEF(Papillon, Dog);
ANIMAL_DEF(Dachshund, Dog);

ANIMAL_DEF(Cat, Animal);
ANIMAL_DEF(MaineCoon, Cat);
ANIMAL_DEF(Munchkin, Cat);
ANIMAL_DEF(Birman, Cat);


struct MySiblingPolicy
{
	template <typename T>
	struct apply { typedef std::vector<T> type; };
};

struct MyChildrenPolicy
{
	template <typename K, typename V>
	struct apply { typedef boost::unordered_map<K, V> type; };
};

struct Printer
{
	template <typename T>
	void operator ()(const T& arg) const { std::cout << arg->name() << std::endl; }
};

struct Clearer
{
	template <typename T>
	void operator ()(T& list) const { list.clear(); }
};

template <typename Tree>
void print(const Tree& tree)
{
	Printer p;

	std::cout << "all ------------------" << std::endl;
	tree.for_each_extended(boost::cref(p));
	std::cout << std::endl;

	std::cout << "dogs -----------------" << std::endl;
	tree.template for_each_extended<Dog>(Printer());
	std::cout << std::endl;

	std::cout << "cats -----------------" << std::endl;
	tree.template for_each_extended<Cat>(Printer());
	std::cout << std::endl;
}

int main()
{
	typedef mel::hierarchy_tree<
		boost::shared_ptr<Animal>,
		MySiblingPolicy,
		MyChildrenPolicy
	> tree_t;
	
	tree_t tree;

	// dogs
	tree.insert(BorderCollie::create());
	tree.insert(Papillon::create());
	tree.insert(Dachshund::create());

	// cats
	tree.insert(MaineCoon::create());
	tree.insert(Munchkin::create());
	tree.insert(Birman::create());

	// print
	print(tree);

	// clear dogs
	tree.for_each<Dog>(Clearer());

	// print
	print(tree);

	// extract cats
	std::deque<CatPtr> cats;
	tree.for_each<Cat>(mel::make_hierarchy_tree_extractor(cats));
}

/* output */
/*
all ------------------
Animal -> Cat -> Munchkin
Animal -> Cat -> Birman
Animal -> Cat -> MaineCoon
Animal -> Dog -> Papillon
Animal -> Dog -> BorderCollie
Animal -> Dog -> Dachshund

dogs -----------------
Animal -> Dog -> Papillon
Animal -> Dog -> BorderCollie
Animal -> Dog -> Dachshund

cats -----------------
Animal -> Cat -> Munchkin
Animal -> Cat -> Birman
Animal -> Cat -> MaineCoon

all ------------------
Animal -> Cat -> Munchkin
Animal -> Cat -> Birman
Animal -> Cat -> MaineCoon

dogs -----------------

cats -----------------
Animal -> Cat -> Munchkin
Animal -> Cat -> Birman
Animal -> Cat -> MaineCoon
*/
