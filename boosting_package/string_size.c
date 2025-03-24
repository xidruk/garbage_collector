#include <boosting_package.h>

size_t	string_size(const char *ptr)
{
	size_t _Size = 0;
	if (!ptr)
	{
		return (0);
	}
	while (ptr[_Size])
	{
		_Size++;
	}
	return (_Size);
}