#include "HashedString.h"

#include <string.h>

unsigned long
HashedString::hash(const char *key)
{
    unsigned long hash = 0;
    size_t i;
	unsigned int key_len = strlen(key);

    for (i = 0; i < key_len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

