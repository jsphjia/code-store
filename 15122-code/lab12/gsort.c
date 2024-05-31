#include <stdlib.h>
#include <stdbool.h>
#include "lib/gsort.h"
#include "lib/contracts.h"

void gsort(void *A, size_t count, size_t elt_size,
           compare_fn *cmp, swap_fn *swap) {
  REQUIRES(A != NULL && cmp != NULL && swap != NULL);  // UPDATE ME
  // REQUIRES(sizeof(A) >= count * elt_size);

  /* WRITE ME */
  for(size_t i = 0; i < count; i++)
  {
    size_t min = i;
    for(size_t j = i + 1; j < count; j++)
    {
      if(cmp(&(((char*) A)[j * elt_size]), &(((char*) A)[min * elt_size])) < 0) min = j;
    }
    swap(&((char*) A)[i * elt_size], &((char*) A)[min * elt_size]);
  }

  return;
}
