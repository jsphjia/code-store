#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/xalloc.h"

int main ()
{
  /* String allocations */                      /* Fill in each blank with *
                                                 * STACK, HEAP, or DATA    */
  char s1[] = "Hello";                          // ____________
  char s2[] = {'H', 'e', 'l', 'l', 'o', '\0'};  // ____________
  char *s3 = "Hello";                           // ____________
  char *s4 = xmalloc(sizeof(char)*6);           // ____________
  strcpy(s4, s2);                               // Copies "Hello" into s4
  (void)s1; (void)s2; (void)s3; (void)s4;       // Ignore this line

  /* Writing to string */     /* Fill in each blank with the error caused *
                               * by the bad line, if applicable           */
  s2[0] = 'B';                // _______________
  // s3[2] = 'a';                // _____trying to modify a string literal__________
  s4[4] = 'k';                // _______________
  // s1[5] = 'e';                // _______causes unexpected error________
  // s4[6] = 'o';                // _____out of bounds index__________

  /* Printing strings */      /* Fill in each blank with the error caused *
                               * by the bad line, if applicable           */
  printf("s1: %s\n", s1);     // _______________
  printf("s2: %s\n", s2);     // _______________
  printf("s3: %s\n", s3);     // _______________
  printf("s4: %s\n", s4);     // _______________

  /* Freeing strings */       /* Which strings need to be freed? */
  // INSERT FREES HERE
  free(s4);

  return 0;
}
