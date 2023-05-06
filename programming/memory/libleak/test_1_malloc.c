#include <malloc.h>

int main()
{
	char *str1 = malloc(256);

	free(str1);
}
