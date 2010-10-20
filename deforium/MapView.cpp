#include <cstdlib>

#include "MapView.h"

using namespace boost;

MapView::MapView(const Extents& extents,
				 GameState *gameState,
				 Map *map)
	: _extents(extents),
	  _gameState(gameState),
	  _map(map)
{
	_maxWidthPixels = gameState->normalUnitSize * _map->getWidth();
	_maxHeightPixels = gameState->normalUnitSize * _map->getHeight();
}

Extents
MapView::getExtents() const
{
	return _extents;
}

bool MapView::setExtents(const Extents& extents)
{
	if(extents.getMinX() < 0 || extents.getMinY() < 0 || 
	   extents.getMaxX() > _maxWidthPixels ||
	   extents.getMaxY() > _maxHeightPixels)
	   return false;

	_extents = extents;

	getChangeSignal()(_extents);

    return true;
}
/*
void MapView::offsetExtents(int x, int y)
{
	_extents.setMinX(_extents.getMinX() + x);
	_extents.setMinY(_extents.getMinY() + y);

    // notify slots of changes
	getChangeSignal()(_extents);
}
*/
signal<void (const Extents&)>&
MapView::getChangeSignal()
{
	return _changeSignal;
}
