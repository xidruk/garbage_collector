#ifndef GHEADER_H
#define GHEADER_H

//---> includes 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gdefs.h"


//---> define functions 
void set_tracker_to_0(_GC_memory_c *mem_container);
_GC_memory_c *init_mem_c(void);
int resize_mem_container(_GC_memory_c *mem_container);
void *gc_malloc(_GC_memory_c *mem_container, size_t size);
int gc_free(_GC_memory_c *mem_container, void *target);
int gc_free_all(_GC_memory_c *mem_container);
void gc_destroy(_GC_memory_c *mem_container);

//---> errorm functions

errorm	*init_errorm(void);
void	set_error(errorm *error, unsigned int status, const char *message);
void	free_errorm(errorm *error);

//---> function api 
//---> function api 

int init_gc(_GC_memory_c **holder, errorm *error_stat);
void *gc_alloc(errorm *error_stat, _GC_memory_c *memory_container, size_t size);
int gc_fr(errorm *error_stat, _GC_memory_c *memory_container, void *target);
int gc_free_ev(errorm *error_stat, _GC_memory_c *memory_container);
int gc_destroy_api(errorm *error_stat, _GC_memory_c **memory_container);

#endif