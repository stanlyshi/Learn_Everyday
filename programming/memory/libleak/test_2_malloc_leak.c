#include <malloc.h>

int main()
{
	char *str1 = malloc(256);
	str1 = NULL;
	str1 = malloc(256);
	free(str1);
}
