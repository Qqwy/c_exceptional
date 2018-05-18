#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> Simple try/catch/finally without raise\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("Inside try\n");
    ++testvar;
  } catch(err) {
    printf("Catch: Should not be run, but was run with error code: %d\n", err);
    assert(false);
  } finally {
    printf("Finally: Should be run after try\n");
    ++testvar;
  }
  ++testvar;
  assert(testvar == 4);
}
