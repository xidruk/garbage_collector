#include <error_manager.h>

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
