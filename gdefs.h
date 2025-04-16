#ifndef GDEFS_H
#define GDEFS_H

//---> define error state

//---> GC <garbae collector errors>

typedef struct gc_memory
{
    unsigned long container_size;
    void **memory_container;
    unsigned char *memory_tracker;
} _GC_memory_c;

//---> errorm defs :
#define PRE_MSGSIZE 1024

typedef struct error_manager
{
    unsigned int error_status; // status from 0 to n
    char *error_message;
} errorm;

// errors state defs

#define ALLOC_FAILS 0
#define GC_MALLOC_FAILS 1
#define TARGET_NOT_FOUND 2
#define ALREADY_FREED 3
#define NULL_PARAM 4
#define GC_FREE_ALLFAILS 5
#define UNEXPECTED_ERROR 6

//---> function api defs 

#endif