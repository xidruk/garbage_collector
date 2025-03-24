#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

//---> includes 

#include <stdlib.h>
#include <../boosting_package/boosting_package.h>

//---> type defs 

#define PRE_MSGSIZE 1024

typedef struct error_manager
{
    unsigned int error_status; // status from 0 to n
    char *error_message;
} errorm;

//---> functions 

errorm	*init_errorm(void);
void	set_error(errorm *error, unsigned int status, const char *message);
void	free_errorm(errorm *error);

#endif
