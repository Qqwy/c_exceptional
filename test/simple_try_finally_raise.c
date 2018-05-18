#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> Simple try/finally with raise\n");

  int testvar = 0;
  ++testvar;
  try {
    printf("Inside try\n");
    ++testvar;
    throw(42);
    printf("After throw: Should not print");
    assert(false);
  } finally {
    printf("Finally: Should be run after try\n");
    ++testvar;
  }

  ++testvar;
  assert(testvar == 4);
}
