#include "../gdefs.h"
#include "gheader.h"

void *gc_malloc(_GC_memory_c *mem_container, size_t size)
{
    unsigned long I_dx = 0;
    void *ptr;

    if (!mem_container || size == 0)
        return (NULL);
    
    // searching for a empty slot to use
    for (I_dx = 0; I_dx < mem_container->container_size ; I_dx++)
    {
        if (mem_container->memory_tracker[I_dx] == 0)
        {
            ptr = malloc(size);
            if (!ptr)
                return (NULL);
            mem_container->memory_container[I_dx] = ptr;
            mem_container->memory_tracker[I_dx] = 1;
            return (ptr);
        }
    }
    if (resize_mem_container(mem_container) != 0)
        return (NULL);
    
    // allocate memory and set the pointer to the slot we found 
    I_dx = mem_container->container_size / 2;
    ptr = malloc(size);
    if (!ptr)
        return (NULL);
    mem_container->memory_container[I_dx] = ptr;
    mem_container->memory_tracker[I_dx] = 1;
    return (ptr);
}
