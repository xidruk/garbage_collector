#include "../gdefs.h"
#include "../gheader.h"

// free everything 

int gc_free_ev(errorm *error_stat, _GC_memory_c *memory_container)
{
    if (!error_stat || !memory_container)
    {
        if (error_stat)
        {
            set_error(error_stat, NULL_PARAM, "[NULL-Param] - The Passed Param is NULL - func-ref:{gc_free_ev} - file-ref{func-api/gc_free_ev.c}");
            return (-1);
        }
        return (-1);
    }
    if (gc_free_all(memory_container) != 0)
    {
        set_error(error_stat, GC_FREE_ALLFAILS, "[Free-All-Failed] - Ref-func{gcfree_all}-file_ref{func_api/gc_free_ev.c}");
        return (-2);
    }
    return (0);
}
