#ifndef EXCEPTIONAL_H_
#define EXCEPTIONAL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <setjmp.h>

// Define this to print debug information about stack winding/unwinding.
#ifdef DEBUG_EXCEPTIONAL
  #define _Exceptional_debug_print(...) fprintf(stderr, "Exceptional: " __VA_ARGS__);
#else
  #define _Exceptional_debug_print(...) ;
#endif

#define _Exceptional_LINE_LABEL_IMPL(x, y) _Exceptional_Label_ ## x ## y
#define _Exceptional_LINE_LABEL(x, y) _Exceptional_LINE_LABEL_IMPL(x, y)

extern jmp_buf _Exceptional_env;
extern int _Exceptional_try_block_nesting_count;

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
      For this, the `_Exceptional_LINE_LABEL(name, __LINE__)` macro is used.
  ```
*/
#define try while(1)                                                    \
    if(0){                                                              \
    _Exceptional_LINE_LABEL(try_block_finished, __LINE__):                         \
      break;                                                            \
    } else                                                              \
      for(jmp_buf _Exceptional_env_backup;;)                          \
        if(0) {                                                         \
        _Exceptional_LINE_LABEL(try_block_unwind, __LINE__):                       \
          --_Exceptional_try_block_nesting_count;                      \
          memcpy(_Exceptional_env, _Exceptional_env_backup, sizeof(jmp_buf)); \
          _Exceptional_debug_print("Unwinding stack: %p\n", &_Exceptional_env_backup); \
          goto _Exceptional_LINE_LABEL(try_block_finished, __LINE__);              \
        }else                                                           \
          for(int _Exceptional_exception_code = 0;;) \
            for(int _Exceptional_exception_block_dispatcher = 0;;)     \
              while(1)                                                  \
                if(_Exceptional_exception_block_dispatcher)            \
                  goto _Exceptional_LINE_LABEL(try_block_unwind, __LINE__);        \
                else if(1){                                             \
                  _Exceptional_debug_print("Winding stack: %p\n", &_Exceptional_env_backup); \
                  memcpy(_Exceptional_env_backup, _Exceptional_env, sizeof(jmp_buf)); \
                  ++_Exceptional_try_block_nesting_count;              \
                  goto _Exceptional_LINE_LABEL(try_body_second_half, __LINE__);    \
                } else                                                  \
                _Exceptional_LINE_LABEL(try_body_second_half, __LINE__):           \
                  for(_Exceptional_exception_code = setjmp(_Exceptional_env); _Exceptional_exception_block_dispatcher < 3; ++_Exceptional_exception_block_dispatcher) \
                    if(_Exceptional_exception_code == 0 && _Exceptional_exception_block_dispatcher == 0)

/*
  This block is only executed if we returned from a `longjmp` (thrown by `throw`) previously.

  The `for`-construct exists in here to ensure that a finally-block will be run if it is added at the end.
*/
#define catch(exception) else                                           \
    for(int _Exceptional_rethrown_exception_code = 0; _Exceptional_exception_block_dispatcher < 3;) \
      for(int exception = _Exceptional_exception_code; _Exceptional_exception_block_dispatcher < 3; ++_Exceptional_exception_block_dispatcher) \
        if(_Exceptional_exception_block_dispatcher % 2 == 0 /* zero or two */) { \
          if(_Exceptional_exception_code && _Exceptional_exception_block_dispatcher == 0) { \
            _Exceptional_debug_print("Winding catch stack: %p\n", &_Exceptional_env_backup); \
            _Exceptional_rethrown_exception_code = setjmp(_Exceptional_env); \
            if(!_Exceptional_rethrown_exception_code) {                 \
              goto _Exceptional_LINE_LABEL(catch_body, __LINE__);       \
            }                                                           \
          }else if(_Exceptional_exception_code && _Exceptional_exception_block_dispatcher == 2){ \
            _Exceptional_debug_print("Unwinding catch stack: %p\n", &_Exceptional_env_backup); \
            memcpy(_Exceptional_env, _Exceptional_env_backup, sizeof(jmp_buf)); \
            if(_Exceptional_rethrown_exception_code){                   \
              _Exceptional_debug_print("Re-throwing error %d after finally block was run\n", _Exceptional_rethrown_exception_code); \
              throw(_Exceptional_rethrown_exception_code);              \
            }                                                           \
          }                                                             \
        } else if(0)                                                    \
        _Exceptional_LINE_LABEL(catch_body, __LINE__):

/*
  Increments the dispatcher to ensure that the finally-block is never executed more than once in a row,
  regardless of whether a `catch`-block exists or not.

  The inline `memcpy` is here to ensure that even if we jump out of a `finally`-block (like when re-throwing), the exception state is still returned to its original version.
*/
#define finally else if(_Exceptional_exception_block_dispatcher == 1 && memcpy(_Exceptional_env, _Exceptional_env_backup, sizeof(jmp_buf)))

/*
  Throws an exception by using `longjmp`.
  When called outside of any `try`-block, will terminate the program with an error message.
*/
#define throw(exception) do {                         \
    if(!_Exceptional_try_block_nesting_count){       \
      fprintf(stderr,"Uncaught exception: %d\n", exception);  \
      exit(EXIT_FAILURE);                             \
    }                                                 \
    _Exceptional_debug_print("Throwing exception: %d\n", exception);  \
    longjmp(_Exceptional_env, exception);            \
  } while (0)                                         \


#define rethrow throw(__Exceptional_exception_code)


#endif //EXCEPTIONAL_H_
