#ifndef __IEVENT_H__
#define __IEVENT_H__

#include <vector>
#include <boost/any.hpp>

/** Interface that defines an event type that can be passed to an
 * IEventAcceptor. */
class IEvent {
public:
	virtual ~IEvent() {}

	/** Should return a meaningful designation for what type of event
	 * this is: (Mouse, Keyboard, etc for User Event), (Move,
	 * Explode, etc for Object Events). */
	virtual int getType() const = 0;

	/** Returns the payload of this event as a list of arbitrary
	 * objects. Based on the type this payload should be more
	 * meaningful. */
	virtual std::vector< boost::any > getData() const = 0;
};

#endif	// __IEVENT_H__
