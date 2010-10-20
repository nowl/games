#ifndef __GAME_UNIT_H__
#define __GAME_UNIT_H__

#include <string>
#include <vector>

#include "Renderable.h"
#include "Updateable.h"
#include "IEventAcceptor.h"
#include "Event.h"
#include "GameState.h"
#include "ObjectTransform.h"
#include "Extents.h"
#include "Log.h"
#include "Geometry.h"

/** Abstract class representing a game object. This object is
 * Renderable, Updateable, and can accept external events. The
 * GameObject class has a concept of a "layer" which determines which
 * rendering layer this GameObject belongs to. During the rendering
 * cycle each layer is rendered in turn (obtained from the GameState)
 * which will determine if this object is rendered or not. The
 * specificRender method should be overridden by a derived class. The
 * children of this object and all derived objects will be of type
 * GameObject.
 */
class GameObject : public Renderable<GameObject>,
				   public Updateable<GameObject>,
				   public NullEventAcceptor
{
public:
	/** Constructor. A GameObject takes a name for it's definition.
	 * @param name the name to give to this GameObject
	 */
    GameObject(const std::string& name, const int layer);

    virtual ~GameObject();

	/** This should return the object transformation for this
	 * GameObject. This specifies the primary way to configure views
	 * and other types of transformations.
	 */
	const ObjectTransform& getObjectTransform();

	/** This sets the object transformation to the given
	 * ObjectTransform. It will also make the screen position of this
	 * object stale and set all children's ObjectTransformation to
	 * this transform.
	 */
	void setObjectTransform(const ObjectTransform& transform);

	/** Returns the name given to this object when it was defined. */
    std::string getName() const;
	
	/** Renders the specificRender if the current layer being rendered
	 * matches the layer of this object and the object is
	 * visible. Then it calls the base-class render routine.
	 */
	virtual void render(GraphicsEngine *graphicsEngine, 
                        GameState *gameState,
                        double interpolation);

	/** Calls specificUpdate on the GameObject and then calls update
	 * on all children.
	 */
	virtual void update(GameState *gameState);

	/** The world position of this GameObject. */
    const Position& getWorldPosition();

	/** Sets the world position of this GameObject. */
    virtual void setWorldPosition(const Position& position);

	/** The screen position of this GameObject. */
    Position getScreenPosition();

	/** Set visibility of this and all children. */
    void setVisible(bool value);

    /** Set the clipping region during the render. */
    void setClipping(const Extents& extents);

    /** Returns the bounding box of this object. */
    virtual Extents getBBox();

    /** Tests a collision with the current object and the passed in
     * object.
     */
    virtual void collideWithObject(GameState *gameState,
                                   GameObject *gameObject);

	/** Get visibility of this and all children. */
    bool getVisible() const;

protected:
	/** This should be overridden by a derived class to specify how to
	 * render this object to the screen.
	 */
	virtual void specificRender(GraphicsEngine *graphicsEngine, 
								GameState *gameState,
								double interpolation);

	/** This should be overridden by a derived class to specify how to
	 * update this object.
	 */
	virtual void specificUpdate(GameState *gameState);

	/** Returns the current rendering layer of this object. */
	int getLayer();

    int _unitSize;

private:
    Log *_log;
    std::string _name;
	int _layer;
    bool _visible;
	ObjectTransform _objectTransform;
	Position _worldPosition;
	Position _screenPosition;
	bool _screenPositionStale;
    int _clipULX, _clipULY;
    int _clipWidth, _clipHeight;

public:
	// Override this so that if a child is newly added then it needs
	// to have it's ObjectTransform set also.
	virtual void addChild(GameObject *child);
};

#endif	// __GAME_UNIT_H__
