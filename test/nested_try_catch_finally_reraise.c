#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../src/exceptional/exceptional.h"

int main(){

  printf("-> Nested try/catch/finally with raise in inner try and re-raise in inner catch\n");

  volatile int testvar = 0;
  ++testvar;
  try {
    printf("Inside outer try\n");
    ++testvar;
    try {
      printf("Inside inner try\n");
      ++testvar;
      throw(42);
      assert(false);
    } catch(inner_err) {
      printf("Inner Catch: Should be run, thrown error code: %d\n", inner_err);
      assert(inner_err == 42);
      ++testvar;
      throw(inner_err);
      printf("Inner Catch: Should not be run");
      assert(false);
    } finally {
      printf("Inner finally: Should be run after inner catch\n");
      assert(testvar == 4);
      ++testvar;
    }
  } catch(err) {
    printf("Outer Catch: Should be run, thrown error code: %d\n", err);
    assert(err == 42);
    ++testvar;
  } finally {
    printf("Outer Finally: Should be run after outer catch\n");
    assert(testvar == 6);
    ++testvar;
  }
  ++testvar;
  assert(testvar == 8);
}
