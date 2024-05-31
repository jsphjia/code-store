/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/bitvector.h"
#include "board-ht.h"

// You are welcome to define helper functions for your tests

int main() {
  hdict_t H = ht_new(10);
  struct board_data *B = xcalloc(1, sizeof(struct board_data));
  B->board = 0x1;

  ht_insert(H, B);
  ASSERT(ht_lookup(H, B->board) == B);

  struct board_data *C = xcalloc(1, sizeof(struct board_data));
  C->board = 0x14;
  ASSERT(ht_lookup(H, C->board) == NULL);
  ht_insert(H, C);
  ASSERT(ht_lookup(H, C->board) == C);

  // struct board_data *D = xcalloc(1, sizeof(struct board_data));
  // D->board = 0x14;
  // ASSERT(ht_lookup(H, D->board) == C);
  // ht_insert(H, D);
  // ASSERT(ht_lookup(H, D->board) == D);
  // ASSERT(ht_lookup(H, C->board) == D);

  printf("All tests passed!\n");
  return 0;
}
