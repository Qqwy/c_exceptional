#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> Simple try/catch without raise\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("Inside try\n");
    ++testvar;
  } catch(err) {
    printf("Catch: Should not be run, but was run with error code: %d\n", err);
    assert(false);
  }
  ++testvar;
  assert(testvar == 3);
}
