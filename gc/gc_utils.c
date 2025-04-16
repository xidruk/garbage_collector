#include "../gdefs.h"
#include "../gheader.h"

void set_tracker_to_0(_GC_memory_c *mem_container)
{
    unsigned long I_dx;

    I_dx = 0;
    if (!mem_container)
        return ;
    while (I_dx < mem_container->container_size)
    {
        mem_container->memory_tracker[I_dx] = 0;
        I_dx++;
    }
}

// init the memory container with a 256 pointer <256 * 8> ? 2048 byte // swaped to 128 ? 512 byte 
_GC_memory_c *init_mem_c(void)
{
    _GC_memory_c *init_memory;

    init_memory = malloc(sizeof(_GC_memory_c));
    if (!init_memory)
        return (NULL);
    // init presize with 512 byte
    init_memory->container_size = 128;
    init_memory->memory_tracker = malloc(init_memory->container_size + 1);
    if (!init_memory->memory_tracker)
    {
        free(init_memory);
        return NULL;
    }
    set_tracker_to_0(init_memory);
    init_memory->memory_tracker[init_memory->container_size] = '\0';
    init_memory->memory_container = (void **)malloc(sizeof(void *) * init_memory->container_size);
    return (init_memory);
}
