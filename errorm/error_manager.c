#include "../gdefs.h"
#include "../gheader.h"


static size_t string_size(const char *ptr)
{
    size_t i = 0;
    while (ptr[i]) i++;
    return i;
}

static void memory_copy(void *target, const void *src, size_t size)
{
    for (size_t i = 0; i < size; i++)
        ((char *)target)[i] = ((const char *)src)[i];
}

errorm	*init_errorm(void)
{
	errorm *errorm_init;

	errorm_init = malloc(sizeof(errorm));
	if (!errorm_init)
	{
		return (NULL);
	}
	errorm_init->error_status = 0;
	errorm_init->error_message = (char *)malloc(PRE_MSGSIZE * sizeof(char));
	if (!errorm_init->error_message)
	{
		free(errorm_init);
		return (NULL);
	}
	return (errorm_init);
}

void	set_error(errorm *error, unsigned int status, const char *message)
{
	if (!error || !message)
	{
		return ;
	}
	error->error_status = status;
	size_t message_size;
	message_size = string_size(message);
	if (message_size >= PRE_MSGSIZE)
	{
		message_size = PRE_MSGSIZE - 1;
	}
	memory_copy(error->error_message, message, message_size);
	error->error_message[message_size] = '\0';
}
 
void	free_errorm(errorm *error)
{
	if (!error)
	{
		return ;
	}
	if (error->error_message)
	{
		free(error->error_message);
	}
	free(error);
}
