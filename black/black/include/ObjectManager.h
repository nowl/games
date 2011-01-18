#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

class ObjectManager
{
private:
    // non-copyable
    ObjectManager(const ObjectManager&);
    ObjectManager operator=(const ObjectManager&);

public:
    ObjectManager();
    ~ObjectManager();

};

#endif // __OBJECT_MANAGER_H__
