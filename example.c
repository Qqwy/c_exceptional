#include <stdio.h>

#include "exceptional.h"

int main(){
  int foo = 0;
  try {
    foo += 1;
    printf("foo try: %d\n", foo);
    try {
      throw(44);
      printf("After inner throw\n");
    } catch (err_inner) {
      printf("Catching inner error %d\n", err_inner);
    } finally {
      printf("Inner finally\n");
    }
    throw(43);
  } catch(err) {
    printf("foo catch: %d, err: %d\n", foo, err);
  } finally {
    printf("Foo finally %d\n", foo);
  }

  return 0;
}
