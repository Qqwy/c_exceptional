# Exceptional

A simple Exception-handling library for C99 and up, that uses some fancy macros for true `try{...}catch(err){...}finally{...}` syntax!


First, include `exceptional/exceptional.h` in your code.

Then, use the `try { ... }` macro (combined with `catch { ... }` and/or possibly `finally { ... }`) to guard against errors. 

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

## Usage Notes:

- Don't use return statements inside a `try`, `catch` or `finally` block, because it will forego the proper cleanup of the exception state.
- When altering a variable that was introduced in the same function but outside the `try/catch/finally` block, be sure it is marked as `volatile`. This is because Exceptional is implemented using `setjmp`+`longjmp`, which might clobber the variables' values unless they were marked like this. (While `gcc` and other common comilers seem to do the right thing even when `volatile` was not used, you should not depend on it).
- Re-throwing from within a catch-block now is fully supported! (**Hooray!**) The related finally block will be run first before bubbling the exception up the stack.

**Truly exceptional, isn't it?**


# Running Tests

1. Clone or download the repository.
2. Make sure that besides `gcc` you have `nodejs` and the node package manager (`npm`) installed on your system, because tests are run using the tool `urchin`.
2. Run `make test`. This will:
  - auto-install `urchin` if it was not installed yet, 
  - Compile and link all C-files in the `/test` folder
  - Run `urchin` on these executables. Urchin tests are successful if the exit code of the executable == 0.
  - Clean up the generated executables afterwards.


# Planned Future work:

1. Rewriting the throwing/catching logic because the way a values is currently read from `setjmp` is not fully spec-adherent (i.e. it relies on undefined behaviour), although all common compilers allow it.
2. I am currently thinking about adding support to throw any (user supplied) structure rather than just integers.

How does it work? Or: Demystifying the Black Magic:
====================================

_For a more gradual introduction to these concepts, read ['Metaprogramming custom control structures in C'](https://www.chiark.greenend.org.uk/%7Esgtatham/mp/) by Simon Tatham, which is really good!_

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


Attribution
=============

I want to thank DSMan195276 [whose post on Reddit](https://www.reddit.com/r/C_Programming/comments/6o7kpb/raii_in_c_cleanup_gcc_compiler_extension/dkgzztq/) I came across,
which taught me most of the macro-twiddling strategies used in this library.

He has since pointed me to this marvellous resource, which explains a lot of these techniques in more detail: https://www.chiark.greenend.org.uk/%7Esgtatham/mp/
