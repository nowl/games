#ifndef __CHILDABLE_H__
#define __CHILDABLE_H__

#include <boost/foreach.hpp>

#include <iostream>

#include <vector>

/** Abstract class allowing a derived class to contain "children". The
 * list of children can be any arbitrary number but must be of the
 * same type as the derived class. Each child is also allowed to have
 * a single parent that is also of the same type.
 */
template<class T>
class Childable {
public:
	Childable() : _parent(NULL) {}

	virtual ~Childable() {}

	/** Adds a child to the list of children. */
	virtual void addChild(T *child) { 
		child->setParent(this);
		_children.push_back(child);
	}

	/** Returns a list of the children of this class. */
    std::vector<T*>& getChildren() { return _children; }

	/** Clears the list of children. */
	void clearChildren() {
		BOOST_FOREACH(T* child, _children)
			child->setParent(NULL);
		_children.clear();
	}

	/** Returns the parent of this node. */
	T* getParent() { return _parent; }

	/** Sets the parent of this node. */
	void setParent(Childable<T>* parent) { _parent = parent; }
	
private:
	std::vector<T*> _children;
	Childable<T>* _parent;
};

#endif  // __CHILDABLE_H__
