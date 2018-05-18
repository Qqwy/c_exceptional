#include <stdio.h>
#include <assert.h>

#include "../src/exceptional/exceptional.h"

int main(){
  printf("-> Simple try/finally without raise\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("Inside try\n");
    ++testvar;
  } finally {
    printf("Finally: Should be run after try\n");
    ++testvar;
  }
  ++testvar;
  assert(testvar == 4);
}
