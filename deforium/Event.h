#ifndef __EVENT_H__
#define __EVENT_H__

#include "IEvent.h"

/** Primary class defining event types. The different wrapper
 * functions should be used to make this easier to manage based on the
 * type.
 * @see MouseButtonEventWrapper
 * @see MouseMoveEventWrapper
 */
class Event : public IEvent {
public:
	/** The possible event types. */
	enum EventType {
		KEYPRESS,				/**< a user pressed a key */
		KEYRELEASE,				/**< a user released a key */
		MOUSE_PRESS,			/**< a user pressed a mouse button */
		MOUSE_RELEASE,			/**< a user released a mouse button */
		MOUSE_MOVE,				/**< a user moved the mouse */
		QUIT,					/**< a user closed the program */
		RESIZE,					/**< a user resized the window */

		WALL_COLLISION,
		ENEMY_COLLISION,
	};

	/** The possible mouse buttons. */
    enum MouseButton {
        LEFT,
        MIDDLE,
        RIGHT,
        WHEELUP,
        WHEELDOWN
    };

	Event(EventType type, const std::vector< boost::any >& data);
	virtual ~Event() {}

	virtual int getType() const { return _type; }
	virtual std::vector< boost::any > getData() const { return _data; }

private:
	EventType _type;
	std::vector< boost::any > _data;
};

/** A helper class that takes an Event that has a type of MOUSE_PRESS
 * or MOUSE_RELEASE and makes available easy to use functions to get
 * at the underlying event data.
 */
class MouseButtonEventWrapper {
public:
    MouseButtonEventWrapper(const IEvent& event);
	
    Event::MouseButton getButton() const { return _button; }
	unsigned int getX() const { return _x; }
	unsigned int getY() const { return _y; }

private:
    Event::MouseButton _button;
	unsigned int _x, _y;
};

/** A helper class that takes an Event that has a type of MOUSE_MOVE
 * and makes available easy to use functions to get at the underlying
 * event data.
 */
class MouseMoveEventWrapper {
public:
    MouseMoveEventWrapper(const IEvent& event);
	
	unsigned int getX() const { return _x; }
	unsigned int getY() const { return _y; }
    unsigned int getXrel() const { return _xrel; }
	unsigned int getYrel() const { return _yrel; }

private:
	unsigned int _x, _y, _xrel, _yrel;
};

#endif	// __IEVENT_H__
