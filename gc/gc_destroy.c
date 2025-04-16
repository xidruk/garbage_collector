#include "../gdefs.h"
#include "../gheader.h"

/*
===> gc_destroy aka free all the memory allocatd in the container 
*/

void gc_destroy(_GC_memory_c *mem_container)
{
    if (!mem_container)
    {
        return ;
    }
    gc_free_all(mem_container);

    free(mem_container->memory_tracker);
    free(mem_container->memory_container);
    free(mem_container);
}
