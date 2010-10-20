#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

#include <boost/signal.hpp>

#include "Extents.h"
#include "GameState.h"
#include "map.h"

/** Represents a view onto a map. This is a generic view class that
 * maintains an extents object that can be updated. When the extents
 * are updated then all of the subscribers to this class are notified
 * of the change.
 */
class MapView {
public:
	MapView(const Extents& extents, 
			GameState* gameState,
			Map* map);
	
	/** Get the extents of this map view. */
	Extents getExtents() const;

	/** Set the extents to a new value. */
	bool setExtents(const Extents& extents);

	/** Move the extents by x and y. */
//	void offsetExtents(int x, int y);

	/** Gets the change signal that can be subscribed to for update
	 * notifications. */
	boost::signal<void (const Extents&)>& getChangeSignal();

private:
	Extents _extents;	
	GameState *_gameState;
	Map *_map;
	int _maxWidthPixels, _maxHeightPixels;
	boost::signal<void (const Extents&)> _changeSignal;
};

#endif	// __MAP_VIEW_H__
