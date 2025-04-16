#include "../gdefs.h"
#include "../gheader.h"

void *gc_alloc(errorm *error_stat, _GC_memory_c *memory_container, size_t size)
{
    void *ptr_h;

    if (!error_stat || !memory_container || size == 0)
    {
        if (error_stat)
        {
            set_error(error_stat, GC_MALLOC_FAILS, "[Invalid-Args] - Ref-func{gcalloc}-file_ref{func_api/gcalloc.c}");
        }
        return (NULL);
    }
    ptr_h = gc_malloc(memory_container, size);
    if (!ptr_h)
    {
        set_error(error_stat, GC_MALLOC_FAILS, "[GC-Malloc-Fails] - Ref-func{gcalloc}-file_ref{func_api/gcalloc.c}");
        return (NULL);
    }
    return (ptr_h);
}
