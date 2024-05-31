/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"

// You are welcome to define helper functions for your tests

// prints out the bits in bitvector B from 0 to BITVECTOR_LIMIT - 1
void print_bitvector(bitvector B)
{
  printf("zeros: ");
  for(int i = BITVECTOR_LIMIT - 1; i >= 0; i--)
    printf("%d", (B >> i) & 0x1);
  printf("\n\n");
}

// flips a range of bits in bitvector B
// start inclu. + end exclu.
// start < end
bitvector bitvector_fliprange(bitvector B, int start, int end)
{
  REQUIRES(start < end);

  for(int i = start; i < end; i++)
    B = bitvector_flip(B, i);
  return B;
}

int main() 
{
  bitvector zeros = bitvector_new(); // 0x0
  bitvector zeros1 = bitvector_new(); // 0x0

  ASSERT(bitvector_equal(zeros, zeros1)); // 0x0 == 0x0

  // testing to flip bit 0
  ASSERT(!bitvector_get(zeros, 0)); // 0 = false

  zeros = bitvector_flip(zeros, 0); // 0x1
  print_bitvector(zeros); // printing bitvector
  ASSERT(bitvector_get(zeros, 0)); // 1 = true
  ASSERT(!bitvector_equal(zeros, zeros1)); // 0x1 != 0x0

  zeros = bitvector_flip(zeros, 0); // 0x0
  print_bitvector(zeros); // printing bitvector
  ASSERT(bitvector_equal(zeros, zeros1)); // 0x0 == 0x0
  ASSERT(!bitvector_get(zeros, 0)); // 0 = false

  // testing to flip bit BITVECTOR_LIMIT
  ASSERT(!bitvector_get(zeros, BITVECTOR_LIMIT - 1)); // 0 = false

  zeros = bitvector_flip(zeros, BITVECTOR_LIMIT - 1);
  print_bitvector(zeros); // printing bitvector
  ASSERT(bitvector_get(zeros, BITVECTOR_LIMIT - 1)); // 1 = true
  ASSERT(!bitvector_equal(zeros, zeros1));

  zeros = bitvector_flip(zeros, BITVECTOR_LIMIT - 1);
  print_bitvector(zeros); // printing bitvector
  ASSERT(!bitvector_get(zeros, BITVECTOR_LIMIT - 1)); // 0 = false
  ASSERT(bitvector_equal(zeros, zeros1)); // 0x0 == 0x0

  // testing to flip multiple bits
  zeros1 = 0xF;

  zeros = bitvector_fliprange(zeros, 0, 4);
  print_bitvector(zeros);
  ASSERT(bitvector_equal(zeros, zeros1));

  zeros1 = 0xFF;
  zeros = bitvector_fliprange(zeros, 4, 8);
  print_bitvector(zeros);
  ASSERT(bitvector_equal(zeros, zeros1));

  zeros1 = 0x0;
  zeros = bitvector_fliprange(zeros, 0, 8);
  print_bitvector(zeros);
  ASSERT(bitvector_equal(zeros, zeros1));

  printf("All tests passed!\n");
  return 0;
}
