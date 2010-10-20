#ifndef __IUPDATEABLE_H__
#define __IUPDATEABLE_H__

class GameState;
class Position;

/** Interface to an updateable class. The derived class should
 * overwrite the update method in order to reflect a change to the
 * underlying object (or send events to other objects) during each
 * game tick.
 */
class IUpdateable {
public:
    virtual ~IUpdateable() {}

	/** Called once per game tick to update the underlying object. */
    virtual void update(GameState *gameState) = 0;
};

#endif  // __IUPDATEABLE_H__
