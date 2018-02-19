#include <stdio.h>
#include <string.h>

#include <setjmp.h>

static jmp_buf exception_env;

#define try while(1)                                                    \
    if(0){                                                              \
    finished:                                                           \
      break;                                                            \
    } else                                                              \
      for(jmp_buf my_env;;)                                             \
        for(int _exception = 0;;)                                       \
          for(int switcher = 0;;)                                       \
            while(1)                                                    \
          if(switcher)                                                  \
            goto finished;                                              \
          else if(1){                                                   \
            memcpy(my_env, exception_env, sizeof(jmp_buf));             \
            goto second_half;                                           \
          } else                                                        \
          second_half:                                                  \
            for(_exception = setjmp(exception_env); switcher <= 2; ++switcher) \
              if(_exception == 0 && switcher == 0)                      \
 
#define catch(exception) else                                           \
    for(int exception = _exception;switcher <= 2; ++switcher)           \
      if(_exception && switcher == 1)                                   \


#define finally else

#define throw(exception) longjmp(exception_env, exception);

int main(){
  int foo = 0;
  try {
    foo += 1;
    printf("foo try: %d\n", foo);
    throw(42);
    } catch(err) {
    printf("foo catch: %d, err: %d\n", foo, err);
    } finally {
    printf("Foo finally %d\n", foo);
    }

  return 0;
}
