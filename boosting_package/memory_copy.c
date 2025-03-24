#include "boosting_package.h"

void memory_copy(void *target,const void *src, size_t size)
{
    size_t _Idx = 0;
    unsigned char *c_target; // casted target 
    const unsigned char *c_src; // casted src

    if (!target || !src || size == 0)
    {
        return ;
    }
    c_target = (unsigned char *)target;
    c_src = (const unsigned char *)src;

    for (_Idx = 0; _Idx < size; _Idx++)
    {
        c_target[_Idx] = c_src[_Idx];
    }
}
