#ifndef __MESSAGEHANDLER_INTERFACE__
#define __MESSAGEHANDLER_INTERFACE__

#include "Message.h"

class MessageHandlerInterface {
public:
    virtual ~MessageHandlerInterface() {}

    virtual bool handle(const Message& message) = 0;
};

#endif /* __MESSAGEHANDLER_INTERFACE__ */
