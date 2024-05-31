#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/xalloc.h"
#include "lib/contracts.h"

void mirror(char *A, char *res)
{
    REQUIRES(A != NULL);
    REQUIRES(res != NULL);

    int n = strlen(A);
    for(int i = 0; i < n; i++)
    {
        res[i] = A[i];
        res[n+i] = A[n-1-i];
    }
    res[2*n] = '\0';
}

int main()
{
    char A[] = "hi!";
    char *a = xmalloc(sizeof(char) * 7);

    mirror(A, a);

    free(a);

    return 0;
}