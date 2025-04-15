#include "../gdefs.h"
#include "../gheader.h"

int gc_destroy_api(errorm *error_stat, _GC_memory_c **memory_container)
{
    if (!error_stat || !memory_container || !(*memory_container))
    {
        if (error_stat)
        {
            set_error(error_stat, ALLOC_FAILS, "[Invalid-Args] - Ref-func{gc_destroy_api}-file_ref{func_api/gc_destroy_api.c}");
            return (-1);
        }
        return (-1);
    }
    if (gc_free_ev(error_stat, *memory_container) == 0)
    {
        free((*memory_container)->memory_tracker);
        free((*memory_container)->memory_container);
        free(*memory_container);
        *memory_container = NULL;
        return (0);
    }
    else
    {
        set_error(error_stat, UNEXPECTED_ERROR, "[UNEXPECTED_ERROR] - Unexpected error in func_ref:{gc_destroy_api} file_pref:{}");
        return (-1);
    }
    return (-1);
}