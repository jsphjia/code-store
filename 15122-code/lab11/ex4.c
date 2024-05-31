#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/xalloc.h"

char *reverse(char *s) {
  size_t len = strlen(s);
  char *result = xcalloc(len + 1, sizeof(char));  // is the length correct?
  for (size_t i = 0; i < len; i++) {
    result[i] = s[len-i-1];
  }
  result[len] = '\0';
  return result;
}

int main() {

  printf("Our input is: %s\n", input);
  printf("We reverse to: %s\n", rev);

  return 0;
}
