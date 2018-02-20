#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max(a, b) \
  ({__typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a :_b; })
    
#define min(a, b) \
  ({__typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a :_b; })

int is_equal(char *x, char *y){
  return strcmp(x, y) == 0;
}




