#ifndef __IEVENTACCEPTOR_H__
#define __IEVENTACCEPTOR_H__

#include "IEvent.h"

class GameObject;
class GameState;

/** Interface to an object that can accept events. processEvent should
 * be overridden in a derived clas in order to accept an arbitrary
 * event from either the user or another object.
 */
class IEventAcceptor {
public:
	virtual ~IEventAcceptor() {}
	
	/** Called from a different object with a given event. The
	 * function should return true if this object "consumes" the event
	 * otherwise it should return false if the event should continue
	 * on to other objects' that are IEventAcceptor.
	 * @param event the event sent to this object
	 * @param gameState the global GameState
	 * @return if this event was consumed
	 */
	virtual bool processEvent(const IEvent& event, 
							  GameState* gameState) = 0;
};

/** Abstract base class of IEventAcceptor that simply passes on
 * received events (does not consume them).
 */
class NullEventAcceptor : public IEventAcceptor {
public:
	virtual ~NullEventAcceptor() {}
	
	/** Passes on the given event. */
	virtual bool processEvent(const IEvent& event,
							  GameState* gameState)
	{
		return false;
	}
};

#endif	// __IEVENTACCEPTOR_H__
