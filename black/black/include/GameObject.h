#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

class IRenderable;

class GameObject
{
public:
    GameObject();
    ~GameObject();

    IRenderable *Renderer;
};

#endif // __GAME_OBJECT_H__
