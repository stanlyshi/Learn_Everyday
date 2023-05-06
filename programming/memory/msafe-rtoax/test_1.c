#include <stdio.h>
#include "msafe.h"


int main()
{
    printf("%s.\n", msafe_strerror(MSAFE_DISABLED));
    printf("%s.\n", msafe_strerror(MSAFE_OK));
    printf("%s.\n", msafe_strerror(MSAFE_FREE));
    printf("%s.\n", msafe_strerror(MSAFE_HEAD));
    printf("%s.\n", msafe_strerror(MSAFE_TAIL));
    printf("%s.\n", msafe_strerror(123123));
    printf("%s.\n", msafe_strerror(-3));
}
