#ifndef __SUBSYSTEM_MANAGER_H__
#define __SUBSYSTEM_MANAGER_H__

#include <boost/noncopyable.hpp>

class SubSystemManager : private boost::noncopyable {
public:
    virtual ~SubSystemManager() {};

};

#endif  // __SUBSYSTEM_MANAGER_H__
