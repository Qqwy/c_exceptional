#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> Simple try/catch/finally with raise\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("Inside try\n");
    throw(42);
    assert(false);
  } catch(err) {
    printf("Catch: Should be run, thrown error code: %d\n", err);
    assert(err == 42);
    ++testvar;
  } finally {
    printf("Finally: Should be run after catch\n");
    ++testvar;
  }
  ++testvar;
  assert(testvar == 4);
}
