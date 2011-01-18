#ifndef __GROWABLE_STRING_H__
#define __GROWABLE_STRING_H__

struct GrowableString
{
public:
    char *Val;

    GrowableString(size_t capacity = 10)
        : capacity(capacity)
    {
        Val = (char*)malloc(sizeof(*Val) * capacity);
    }

    ~GrowableString()
    {
        free(Val);
    }

    void growTo(size_t cap)
    {
        bool redo = false;

        while(capacity < cap)
        {
            capacity *= 2;
            redo = true;
        }

        if(redo)
            Val = (char*)realloc(Val, sizeof(*Val) * capacity);
    }

private:
    size_t capacity;
};

#endif // __GROWABLE_STRING_H__
