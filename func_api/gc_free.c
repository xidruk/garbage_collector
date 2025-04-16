#include "../gdefs.h"
#include "../gheader.h"

int gc_fr(errorm *error_stat, _GC_memory_c *memory_container, void *target)
{
    int status;

    if (!error_stat || !memory_container || !target)
    {
        if (error_stat)
        {
            set_error(error_stat, ALLOC_FAILS, "[Invalid-Args] - Ref-func{gcfree}-file_ref{func_api/gcfree.c}");
            return (-1);
        }
    }
    status = gc_free(memory_container, target);
    if (status == -1)
    {
        set_error(error_stat, NULL_PARAM, "[Invalid-Args] - Ref-func{gcfree}-file_ref{func_api/gcfree.c}");
        return (-1);
    }
    else if (status == -2)
    {
        set_error(error_stat, ALREADY_FREED, "[Already-Freed] - Ref-func{gcfree}-file_ref{func_api/gcfree.c}");
        return (-2);
    }
    else if (status == -3)
    {
        set_error(error_stat, TARGET_NOT_FOUND, "[Target-Not-Found] - Ref-func{gcfree}-file_ref{func_api/gcfree.c}");
        return (-3);
    }
    return (0);
}
