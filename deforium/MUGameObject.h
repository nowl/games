#ifndef __MUGAMEOBJECT_H__
#define __MUGAMEOBJECT_H__

#include "map.h"
#include "MapView.h"
#include "RIBGameObject.h"

/** Map Unit Game Object. This is meant to be direct children of
 * MapGameObject. These objects automatically handle views and map
 * model updates.
 */
class MUGameObject : public GameObject {
public:
	/** Constructor. Takes ownership of the passed in GameObject
	 * pointer. Adds the passed in GameObject as a child of this
	 * GameObject so that it reflects updates and renders of this
	 * object.
	 */
	MUGameObject(GameObject *go,
				 Map* map,
				 MapView* mapView);

	/** Destructor. Automatic deletion of the originally passed in
	 * GameObject occurs here.
	 */
	virtual ~MUGameObject();

    virtual void collideWithObject(GameState *gameState,
                                   GameObject *gameObject);

    virtual Extents getBBox();
	    
private:
	GameObject *_go;
	Map *_map;
	MapView * _mapView;

	boost::signals::connection _mapConnection;
    boost::signals::connection _mapViewConnection;
	
	void mapChange(const Position& position);
	void mapViewChange(const Extents& extents);
};

#endif	// __MUGAMEOBJECT_H__
