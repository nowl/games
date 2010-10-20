#ifndef __UPDATEABLE_INTERFACE__
#define __UPDATEABLE_INTERFACE__

class UpdateableInterface {
public:
    virtual ~UpdateableInterface() {}

    virtual void update(unsigned int game_ticks) = 0;
};

#endif /* __UPDATEABLE_INTERFACE__ */
