#include <stdlib.h>
#include <stdbool.h>
#include "lib/xalloc.h"

int main()
{
    char A[] = "hi!";
    char B[] = {'h', 'i', '!'};
    char *C = "hi!";
    char *D = xmalloc(sizeof(char) * 4);

    char b[7];
    b[0] = 'a';
    printf("%s\n", b);
    free(D);
}