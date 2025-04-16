#include "../gheader.h"
#include "../gdefs.h"

int resize_mem_container(_GC_memory_c *mem_container)
{
    unsigned long new_size;
    void **new_container;
    unsigned char *new_tracker;

    if (!mem_container)
        return (-1);
    new_size = mem_container->container_size * 2;
    new_tracker = realloc(mem_container->memory_tracker, new_size + 1);
    if (!new_tracker)
        return (-2);
    mem_container->memory_tracker = new_tracker;
    for (unsigned long i = mem_container->container_size ; i < new_size; i++)
    {
        mem_container->memory_tracker[i] = 0;
    }
    mem_container->memory_tracker[new_size] = '\0';
    new_container = realloc(mem_container->memory_container, sizeof(void *) * new_size);
    if (!new_container)
        return (-3);
    mem_container->memory_container = new_container;
    mem_container->container_size = new_size;
    return (0);
}

