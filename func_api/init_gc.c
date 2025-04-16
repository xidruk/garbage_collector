#include "../gdefs.h"
#include "../gheader.h"

int init_gc(_GC_memory_c **holder, errorm *error_stat)
{
    if (!error_stat || !holder)
        return (-1);

    *holder = init_mem_c();
    if (!(*holder))
    {
        set_error(error_stat, ALLOC_FAILS, "[Malloc-Fails] - Ref-func{init_gc}-file_ref{falloc_api.c}");
        return (-2);
    }
    return (0);
}
