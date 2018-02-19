#include <stdio.h>
#include <string.h>

#include <setjmp.h>


#define TP2(x, y) x ## y
#define TP(x, y) TP2(x, y)

static jmp_buf exception_env;

#define try while(1)                                                    \
    if(0){                                                              \
    TP(__try_block_finished, __LINE__):                                 \
      break;                                                            \
    } else                                                              \
      for(jmp_buf my_env;;)                                             \
        if(0) {                                                         \
        TP(__try_block_unwind, __LINE__):                               \
          memcpy(exception_env, my_env, sizeof(jmp_buf));               \
          printf("Unwinding stack: %p\n", &my_env);                     \
          goto TP(__try_block_finished, __LINE__);                      \
        }else                                                           \
          for(int _exception = 0;;)                                     \
            for(int exception_dispatcher = 0;;)                         \
              while(1)                                                  \
                if(exception_dispatcher)                                \
                  goto TP(__try_block_unwind, __LINE__);                \
                else if(1){                                             \
                  printf("  Winding stack: %p\n", &my_env);             \
                  memcpy(my_env, exception_env, sizeof(jmp_buf));       \
                  goto TP(__try_body_second_half, __LINE__);            \
                } else                                                  \
                TP(__try_body_second_half, __LINE__):                   \
                  for(_exception = setjmp(exception_env); exception_dispatcher < 2; ++exception_dispatcher) \
                    if(_exception == 0 && exception_dispatcher == 0)


#define catch(exception) else                                           \
    for(int exception = _exception;exception_dispatcher < 2; ++exception_dispatcher) \
      if(_exception && exception_dispatcher == 0 && memcpy(exception_env, my_env, sizeof(jmp_buf))) \


#define finally else if(++exception_dispatcher)

#define throw(exception) longjmp(exception_env, exception);

int main(){
  int foo = 0;
  try {
    foo += 1;
    printf("foo try: %d\n", foo);
    try {
      printf("_exception: %d, exception_dispatcher: %d\n", _exception, exception_dispatcher);
      throw(44);
    } catch (err_inner) {
      printf("Catching inner error %d\n", err_inner);
      throw(43);
    } finally {
      printf("Inner finally\n");
    }
  } catch(err) {
    printf("foo catch: %d, err: %d\n", foo, err);
  } finally {
    printf("Foo finally %d\n", foo);
  }

  return 0;
}
