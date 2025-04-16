#include "../gdefs.h"
#include "../gheader.h"

int gc_free(_GC_memory_c *mem_container, void *target)
{
    unsigned long I_dx = 0;
    int found = 0;

    if (!mem_container || !target)
        return (-1);
    for (I_dx = 0; I_dx < mem_container->container_size; I_dx++)
    {
        if (mem_container->memory_container[I_dx] == target)
        {
            if (mem_container->memory_tracker[I_dx] == 0)
            {
                return (-2);
            }
            mem_container->memory_tracker[I_dx] = 0;
            free(mem_container->memory_container[I_dx]);
            mem_container->memory_container[I_dx] = NULL;
            found = 1;
            break ;
        }
    }
    if (!found)
    {
        return (-3);
    }
    return (0);
}
