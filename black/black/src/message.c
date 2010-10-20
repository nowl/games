#include "black.h"

struct message *
message_create(game_object *sender, game_object *receiver, message_callback_func callback_func)
{
    struct message *mes = malloc(sizeof(*mes));
    SLL_ENTRY_INIT(mes, _sll_entry);
    mes->sender = sender;
    mes->receiver = receiver;
    mes->callback_func = callback_func;
    return mes;
}

void
message_destroy(struct message *message)
{
    free(message);
}
