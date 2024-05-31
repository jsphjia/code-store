/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include "lib/bitvector.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"

bitvector bitvector_new()
{
  bitvector B = (bitvector) 0x0;
  return B;
}

bool bitvector_get(bitvector B, index_t i)
{
  REQUIRES(i < BITVECTOR_LIMIT);
  // printf("%d: %d\n", i, (B >> i) & 0x1);

  int res = (B >> i) & (bitvector) 0x1; // shift B i bits to the right

  return res != 0;
}

bitvector bitvector_flip(bitvector B, index_t i)
{
  REQUIRES(i < BITVECTOR_LIMIT);
  // printf("flipping %dth bit\n", i);

  bitvector mask = ~(((bitvector)0x1) << i); // mask so only the ith bit is 0 + everything else is 1
  bitvector modiB = mask & B; // modify B w the mask
  bitvector flipB = ((bitvector)(!bitvector_get(B, i))) << i; // get the bit, flip it, + push it to the right spot
  
  return modiB | flipB; // add it in with or operator
}

bool bitvector_equal(bitvector B1, bitvector B2)
{
  // printf("%d %d\n", B1, B2);
  return B1 == B2;
}

void bitvector_free(bitvector B)
{
  (void) B;
}
