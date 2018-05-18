#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> simple try/catch with raise\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("inside try\n");
    ++testvar;
    throw(42);
    assert(false);
  } catch(err) {
    printf("catch: should be run, thrown error code: %d\n", err);
    assert(err == 42);
    ++testvar;
  }
  ++testvar;
  assert(testvar == 4);
}
