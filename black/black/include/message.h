#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "types.h"
#include "list.h"

struct message {
    game_object *sender, *receiver;

    message_callback_func callback_func;

    SLL_ENTRY(message) _sll_entry;
};

struct message *message_create(game_object *sender, game_object *receiver, message_callback_func callback_func);
void message_destroy(struct message *message);

#endif /* __MESSAGE_H__ */
