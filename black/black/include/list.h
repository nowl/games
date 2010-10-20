#ifndef __LIST_H__
#define __LIST_H__

/* created in the same style as the sys/tree.h from bsd */

#define SLL_LIST(name, type)                                            \
    struct name {                                                       \
        struct type *sll_head; /* head of the list */                   \
        struct type *sll_tail; /* tail of the list */                   \
    }

#define SLL_INITIALIZER                 { NULL, NULL }

#define SLL_INIT(list) do {                                             \
        (list)->sll_head = NULL;                                        \
        (list)->sll_tail = NULL;                                        \
    } while (/*CONSTCOND*/ 0)

#define SLL_ENTRY(type)                                                 \
    struct {                                                            \
        struct type *next;          /* next element */                  \
    }

#define SLL_ENTRY_INIT(elm, field)                                      \
    (elm)->field.next = NULL;

#define SLL_NEXT(elm, field)             (elm)->field.next
#define SLL_HEAD(head)                   (head)->sll_head
#define SLL_TAIL(tail)                   (tail)->sll_tail

/* Generates prototypes and inline functions */
#define SLL_PROTOTYPE(name, type, field)                                \
    SLL_PROTOTYPE_INTERNAL(name, type, field, )
#define SLL_PROTOTYPE_STATIC(name, type, field)                         \
    SLL_PROTOTYPE_INTERNAL(name, type, field, __unused static)
#define SLL_PROTOTYPE_INTERNAL(name, type, field, attr)                 \
    attr void name##_SLL_APPEND(struct name *,                          \
                                struct type *);                         \
    attr struct type *name##_SLL_NEXT(struct name *,                    \
                                      struct type *);                   \
    attr struct type *name##_SLL_REMOVE(struct name *,                  \
                                        void *,                         \
                                        int (*)(void *, struct type *)); \
    attr void name##_SLL_CLEAR(struct name *,                           \
                               void (*)(void *));

#define SLL_GENERATE(name, type, field)                                 \
    SLL_GENERATE_INTERNAL(name, type, field, )
#define SLL_GENERATE_STATIC(name, type, field)                          \
    SLL_GENERATE_INTERNAL(name, type, field, __unused static)
#define SLL_GENERATE_INTERNAL(name, type, field, attr)                  \
    attr void name##_SLL_APPEND(struct name *list,                      \
                                struct type *insert)                    \
    {                                                                   \
        SLL_NEXT(insert, field) = NULL;                                 \
        struct type *tail = SLL_TAIL(list);                             \
        if(tail)                                                        \
            SLL_NEXT(tail, field) = insert;                             \
        else                                                            \
            SLL_HEAD(list) = insert;                                    \
        SLL_TAIL(list) = insert;                                        \
    }                                                                   \
    attr struct type *name##_SLL_NEXT(struct name *list,                \
                                      struct type *elem)                \
    {                                                                   \
        if(elem)                                                        \
            return SLL_NEXT(elem, field);                               \
        else                                                            \
            return SLL_HEAD(list);                                      \
    }                                                                   \
    attr struct type *name##_SLL_REMOVE(struct name *list,              \
                                        void *to_remove,                \
                                        int (*cmp)(void *, struct type *)) \
    {                                                                   \
        struct type *t = SLL_HEAD(list), *tar = NULL, *p = t;           \
        while(t) {                                                      \
            if( cmp(to_remove, t) == TRUE) {                            \
                tar = t;                                                \
                break;                                                  \
            }                                                           \
            p = t;                                                      \
            t = SLL_NEXT(t, field);                                     \
        }                                                               \
        if(tar) {                                                       \
            if(p != t) {                                                \
                SLL_NEXT(p, field) = SLL_NEXT(t,field);                 \
                if(t == SLL_TAIL(list))                                 \
                    SLL_TAIL(list) = p;                                 \
            } else {                                                    \
                SLL_HEAD(list) = SLL_NEXT(p, field);                    \
                if(p == SLL_TAIL(list))                                 \
                    SLL_TAIL(list) = NULL;                              \
            }                                                           \
            return t;                                                   \
        }                                                               \
        return NULL;                                                    \
    }                                                                   \
    attr void name##_SLL_CLEAR(struct name * list,                      \
                               void (*destroy)(void *))                 \
    {                                                                   \
        if(destroy) {                                                   \
            struct type *t = SLL_HEAD(list);                            \
            struct type *n = t;                                         \
            while(t) {                                                  \
                n = SLL_NEXT(t,field);                                  \
                destroy(t);                                             \
                t = n;                                                  \
            }                                                           \
        }                                                               \
        SLL_TAIL(list) = SLL_HEAD(list) = NULL;                         \
    }



#endif /* __LIST_H__ */
