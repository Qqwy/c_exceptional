#include <stdio.h>

#include "exceptional/exceptional.h"

int main(){
  int foo = 0;

  printf("-> Simple try/finally without raise\n");
  try {
    printf("Inside try\n");
  } finally {
    printf("Finally: Should be run after try\n");
  }

  printf("-> Simple try/finally with raise\n");
  try {
    printf("Inside try\n");
    throw(42);
    printf("Catch: Should not print");
  } finally {
    printf("Finally: Should be run after try\n");
  }

  printf("-> Simple try/catch without raise\n");
  try {
    printf("Inside try\n");
  } catch(err) {
    printf("Catch: Should not be run, but was run with error code: %d\n", err);
  }

  printf("-> Simple try/catch with raise\n");
  try {
    printf("Inside try\n");
    throw(42);
  } catch(err) {
    printf("Catch: Should be run, thrown error code: %d\n", err);
  }

  printf("-> Simple try/catch/finally without raise\n");
  try {
    printf("Inside try\n");
  } catch(err) {
    printf("Catch: Should not be run, but was run with error code: %d\n", err);
  } finally {
    printf("Finally: Should be run after try\n");
  }

  printf("-> Simple try/catch/finally with raise\n");
  try {
    printf("Inside try\n");
    throw(42);
  } catch(err) {
    printf("Catch: Should be run, thrown error code: %d\n", err);
  } finally {
    printf("Finally: Should be run after catch\n");
  }


  printf("-> Nested try/catch/finally with raise in inner try\n");
  try {
    printf("Inside outer try\n");
    try {
      printf("Inside inner try\n");
      throw(42);
    } catch(inner_err) {
      printf("Inner Catch: Should be run, thrown error code: %d\n", inner_err);
    } finally {
      printf("Inner finally: Should be run after inner catch\n");
    }
    printf("End of outer try\n");
  } catch(err) {
    printf("Outer Catch: Should not be run, thrown error code: %d\n", err);
  } finally {
    printf("Outer Finally: Should be run after outer try\n");
  }

  printf("-> Nested try/catch/finally with raise in inner try and re-raise in inner catch\n");
  try {
    printf("Inside outer try\n");
    try {
      printf("Inside inner try\n");
      throw(42);
    } catch(inner_err) {
      printf("Inner Catch: Should be run, thrown error code: %d\n", inner_err);
      throw(inner_err);
      printf("Inner Catch: Should not be run");
    } finally {
      printf("Inner finally: Should be run after inner catch\n");
    }
  } catch(err) {
    printf("Outer Catch: Should be run, thrown error code: %d\n", err);
  } finally {
    printf("Outer Finally: Should be run after outer catch\n");
  }

  /* try { */
  /*   foo += 1; */
  /*   printf("foo try: %d\n", foo); */
  /*   try { */
  /*     throw(44); */
  /*     printf("After inner throw\n"); */
  /*   } catch (err_inner) { */
  /*     printf("Catching inner error %d\n", err_inner); */
  /*   } finally { */
  /*     printf("Inner finally\n"); */
  /*   } */
  /*   throw(43); */
  /* } catch(err) { */
  /*   printf("foo catch: %d, err: %d\n", foo, err); */
  /* } finally { */
  /*   printf("Foo finally %d\n", foo); */
  /* } */

  return 0;
}
