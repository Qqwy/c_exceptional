# Exceptional

A simple Exception-handling library for C99 and up, that uses some fancy macros for true `try{...}catch(err){...}finally{...}` syntax!


First, include `exceptional/exceptional.h` in your code.

Then, use the `try` macro (combined with `catch` and/or possibly `finally`) to guard against errors. 

Implemented using Wonderful Black Ma(cro)gic. It is written in this convoluted style to ensure that you can use the following syntax:

```C
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


**Truly exceptional, isn't it?**


Demystifying the Black Magic:
====================================

    1. We want someone to be able to pass a block or single-statement after writing the macro. This means the macro should
      a) finish with an `if(true)`
      b) Since we want to do something after this block has returned, it should be inside of a loop.
      c) To make sure that the stuff afterwards is only run _after_ the block, we need to make sure that the invariant is different when the loop is run for the second time.
        An example:
        ```C
        for(int var = 0;var < 2; ++i)
          if(var == 1) {
            //stuff to run afterwards
          } else
        ```
    2. This also means that we cannot introduce any construct that ends with a `}` ourselves.
      - If we want to run multiple lines of code, use:
      ```C
      if(1){
        // your code
        goto label;
      } else
        label:
        //Continue statements that do not end with `}` here.
      ```
    3. Unfortunately, that construct does not allow us to introduce new variables, because they would only exist inside the `if`'s body-scope.
    But we can start a for-loop and use its initializer to create a new variable;
    4. This however does mean that we either need a clever way to make sure the for-loop stops, or we need to have a final 'finishing' section at the very top:
    ```C
      while(1)
        if(0) {
          finished_label:
          break;
        } else
          for(int some_var;;)
            for(int some_other_var;;)
              // with at some point inside a nested for-loop:
                if(something_only_true_when_user_code_already_ran)
                  goto finished_label;
    ```

    5. To ensure that the macro can be used in multiple locations, we need to make our lables unique.
      For this, the `LINE_LABEL(name, __LINE__)` macro is used.

Disclaimer
============

This was written to test the limits of C's macro system as well as my own understanding of low-level system languages.

While it seems to work in the limited tests that I ran on it, it should by no means be considered highly stable.

Enjoy!
