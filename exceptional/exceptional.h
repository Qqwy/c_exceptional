#ifndef EXCEPTIONAL_H_
#define EXCEPTIONAL_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <setjmp.h>

#define LINE_LABEL_IMPL(x, y) x ## y
#define LINE_LABEL(x, y) LINE_LABEL_IMPL(x, y)

extern jmp_buf __Exceptional_env;
extern int __Exceptional_try_block_nesting_count;

/*
  Starts a try-block, in which you can use `throw` to jump out of it and handle errors.

  Implemented using Wonderful Black Ma(cro)gic. It is written in this convoluted style to ensure that you can use the following syntax:

  ```
  try {
    // some code here
    if(something_is_wrong)
      throw(error_code);
    // some code that would only be executed if not thrown
  } catch(error_code) {
    // Handle error here in any way you want.
  } finally {
    // Runs regardless of catching/throwing.
  }
  ```

  Do note:

  - Don't use return statements inside this block, because it will forego the cleanup of the exception state.
  - When re-throwing from within `catch`, the `finally`-block will currently _not_ run. So instead, re-throw from within the finally-block (or after the try has finished), if required.


  Demystifying the Black Magic:

    1. We want someone to be able to pass a block or single-statement after writing the macro. This means the macro should
    a) finish with an `if(true)`
    b) Since we want to do something after this block has returned, it should be inside of a loop.
    c) To make sure that the stuff afterwards is only run _after_ the block, we need to make sure that the invariant is different when the loop is run for the second time.
    2. This also means that we cannot introduce any construct that ends with a `}` ourselves.
    - If we want to run multiple lines of code, use:
    ```
    if(1){
    // your code
    goto label;
    } else
    label:
    ```
    3. Unfortunately, that construct does not allow us to introduce new variables, because they would only exist inside the `if`'s body-scope.
    But we can start a for-loop and use its initializer to create a new variable;
    4. This however does mean that we either need a clever way to make sure the for-loop stops, or we need to have a final 'finishing' section at the very top:
    ```
    while(1)
    if(0) {
    finished_label:
    break;
    } else
    // convoluted statements that introduce variables using for-loops.
    // with at some point inside a nested for-loop:
    goto finished_label;

    5. To ensure that the macro can be used in multiple locations, we need to make our lables unique.
      For this, the `LINE_LABEL(name, __LINE__)` macro is used.
  ```
*/
#define try while(1)                                                    \
    if(0){                                                              \
    LINE_LABEL(__try_block_finished, __LINE__):                         \
      break;                                                            \
    } else                                                              \
      for(jmp_buf __Exceptional_env_backup;;)                           \
        for(int __Exceptional_exception_code = 0;;)                     \
          for(int __Exceptional_exception_block_dispatcher = 0;;)       \
            while(1)                                                    \
              if(__Exceptional_exception_block_dispatcher)              \
                goto LINE_LABEL(__try_block_finished, __LINE__);        \
              else if(1){                                               \
                fprintf(stderr,"  Winding stack: %p\n", &__Exceptional_env_backup); \
                memcpy(__Exceptional_env_backup, __Exceptional_env, sizeof(jmp_buf)); \
                ++__Exceptional_try_block_nesting_count;                \
                goto LINE_LABEL(__try_body_second_half, __LINE__);      \
              } else                                                    \
              LINE_LABEL(__try_body_second_half, __LINE__):             \
                for(__Exceptional_exception_code = setjmp(__Exceptional_env); __Exceptional_exception_block_dispatcher < 4; ++__Exceptional_exception_block_dispatcher) \
                  if(__Exceptional_exception_block_dispatcher == 1){    \
                    /* Run after `try` finished or `throw` happened (before `catch` or `finally`) */ \
                    memcpy(__Exceptional_env, __Exceptional_env_backup, sizeof(jmp_buf)); \
                    fprintf(stderr,"Unwinding stack: %p\n", &__Exceptional_env_backup); \
                  } else if(__Exceptional_exception_block_dispatcher == 0 && __Exceptional_exception_code == 0) \
                    /* user-provided block here. Runs first*/

/*
  This block is only executed if we returned from a `longjmp` (thrown by `throw`) previously.

  The `for`-construct exists in here to ensure that a finally-block will be run if it is added at the end.
  The inline `memcpy` is here to ensure that even if we jump out of a `catch`-block, the exception state is still returned to its old version.
*/
#define catch(exception) else                                           \
    for(int exception = __Exceptional_exception_code; __Exceptional_exception_block_dispatcher > 1 && __Exceptional_exception_block_dispatcher < 4; ++__Exceptional_exception_block_dispatcher) \
      if(__Exceptional_exception_block_dispatcher == 2 && __Exceptional_exception_code) \
        /* user-provided block here. Runs when exception was thrown. */


/*
  Increments the dispatcher to ensure that the finally-block is never executed more than once in a row,
  regardless of whether a `catch`-block exists or not.

  The inline `memcpy` is here to ensure that even if we jump out of a `finally`-block (like when re-throwing), the exception state is still returned to its old version.
*/
#define finally else if(__Exceptional_exception_block_dispatcher == 3) \
  /* user-provided block here. Runs regardless of if exception was thrown. */

/*
  Throws an exception by using `longjmp`.
  When called outside of any `try`-block, will terminate the program with an error message.
*/
#define throw(exception) do {                         \
    if(!__Exceptional_try_block_nesting_count){       \
      fprintf(stderr,"Uncaught exception: %d\n", exception);  \
      exit(EXIT_FAILURE);                             \
    }                                                 \
    longjmp(__Exceptional_env, exception);            \
  } while (0)                                         \

#define rethrow throw(__Exceptional_exception_code)


#endif //EXCEPTIONAL_H_
