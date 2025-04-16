#include "../gdefs.h"
#include "../gheader.h"

int gc_free_all(_GC_memory_c *mem_container)
{
    unsigned long I_dx;

    if (!mem_container)
        return (-1);

    for (I_dx = 0; I_dx < mem_container->container_size ; I_dx++)
    {
        if (mem_container->memory_tracker[I_dx] == 1)
        {
            mem_container->memory_tracker[I_dx] = 0;
            free(mem_container->memory_container[I_dx]);
            mem_container->memory_container[I_dx] = NULL;
        }
    }
    return (0);   
}
