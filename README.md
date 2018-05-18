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

- Don't use return statements inside a `try`, `catch` or `finally` block, because it will forego the proper cleanup of the exception state. Just return after the end of the finally block and everything will be fine :-) .
- When altering a variable that was introduced in the same function but outside the `try/catch/finally` block, be sure it is marked as `volatile`. This is because Exceptional is implemented using `setjmp`+`longjmp`, which might clobber the variables' values unless they were marked like this. (While `gcc` and other common comilers seem to do the right thing even when `volatile` was not used, you should not depend on it).
- Re-throwing from within a catch-block now is fully supported! (**Hooray!**) The related finally block will be run first before bubbling the exception up the stack.

**Truly exceptional, isn't it?**

Post Scriptum: [By looking at the Compiler Explorer](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxACZSAbAQwDtQB9T5UgZ1QFdiyTCADkAUloBmPADMWWWQGpsADQDC2AAoAVAJIB5AHIBBADLsAEu3EAGE5KmK8LTCo3b9x81Zv27DtIuyAz8WEriUuo8BOgMeABGAHQIkdgBjsGh4ZHRsXioKWkZQSwhYW65McQuwEVS6f7%2BpeU5UTyYBFQAtgAO9Y0O9gD0w0oAIpiyLm4ECHg8SkRKvTUsBEpYCfzASi6yJN1MBAUsSkwJAhsxTMgA1koA7i74bMP8LM8KtUklMrKKCbYABCAFUAOLsNSaXSGUxmAJKCLSZyuJRQgAewl6J1QLCYDHYWx27FWLgIECSVIAlEpZGT1rIIDEsMQyMjaNgsZgcacCSAOej2AA1EzsEwAJXBAGVONTIsC/pgGB1EcinFMZuiudjcfjCcTgKS1hSqUlaQqld9ZBlmhrpmjMbq%2BYSzHojNh2GYTMDsBY9ABZLRmCAY0hKACetKdPL1BK9F2V7HVjiUGJT0kjf1RbhjvLx8bdHq9Pr9ofDUe13Pz%2Bq97s93t9/qDIbDkflTRMmAxBEwxDOPV67G2yjzccJmBYADdLfZu73%2B3t1lXnQXCQRiBHhwxUPd2K4YrV2GgPgRZ0MTMMAFRq6UEJjEAiLJhLTcAWgSO/u4ZcT3myAQSMBCUZBWCUfgOgiAA2Ww5mIVBHnEGCllQJQqH4PolCuLDlEIc4FCUBBWDiNw%2B3g4geF%2BTskT0PoGEwbpJ17dBwJ4WolAAdTxVlZH4BglGBZh7iUAMmAgZB4OpYA8GQJIlD0DYFieGoCF7M5fzmJS0GnVBQmYpQYgjeiUKUSceEEWYiI2CNgNAs4IMstwDgYHcvl2HgI3WJgMRAW0TCRJDbEC4L7CRDcIwiAB2RVQqRJRRgM1BGJAjA3AQPtMDVJE5GZJLOnmNh2AWdhHngtgO38uKwoQeDHggMiSGPVL5SkGLKqRBK%2BGStBwjmY4ngEBgWLxBgIoSUiMUwZB%2BH0uQlBYVANjghCWDVcRIvGEDjgA%2Bq2UanrMAtaKsvisZLGI4yGuIQiMqXfCIseJgIps/gnlYAgqMq9bNodAlRqitqqoSiUPkWYhMGAB8SJ4RZUGUUCCAA2phmWtzPoCja1pg4LseoiZUIW3tfLxt98ZYMBRA2BylHBghBDOG5e0Y9ZFhcNjevmRZP13O5w3G0Dqbw54XLpEgIdQuY3BCTBWH4XocKWdLTOrPUDPvXt0aUUmOPSs5wbfVH2NkeDuieQgCug2wEYAwLw0ly3fpciNAo/L8HmF/jprZJj/v3Rbk2ID45OlVC2d7Jh0HDfXUbpE2zc0s57cd0bXZ5pQIBIc5ZAXRXZk3QimEWB0FnS9BqR/ZRwYAR34PBwfQT6/KRSZugjQ9ZAjdj7cE24HlE6TkGJi8qoARjkji3Ee5cuswPFZlQ8bzk/eeVkL58lG54TM7Yth6LfRmGKYrOc9Kwgu6Vo4JMKJQdE5pRGNYRZ7cv%2BCDIQQb0BOphaWLnhAOeOY%2BFLY5Q3PwQ6IV2ob1pNKYIk9J7vRMugVCM8E67CYNnPsislKbweERMGnR6aYEjnsa478%2BIsUXmzPA4Q4bnCUDuVA/QTrIFpDoVCRw7huHMuDRW/V7YxH4LIZQ6CFyPWIOgVmsMWD/UDmcdgIi%2BzJntjg8Mjw3CuCISZDhXCLK8KWkrFwU4Hx4AQUpfAQiMrLkeLrXO9DUCML2GDD4otrr8KmtxJYeBGKayRLQOSt8lIEj4PfGWLAn5WSeFLVghMlwbgwPwYQ90UphNAcgfR/VJwSPjoBF8wVMbIQEBRZUU5MCUROqTPQyg1FvWXMsWR98%2BInF6MZeIB4FYHXDA5IeVUcZBVxpAnKI95THVih1MYL1roHROsARaqFmDjQYOeHpG1TIqkyqM%2BhiYGDdLir0iBVUpByVBCwA4j4PjHGVBGO2ETtLVASRsJBpT5qLXOC5BCrETLkngugBJ6jMCPCUEYmoFx6I8D5lNJg1NJYPQ/lhaREVuwLEUmE6hjlgE2hghTLmGAIz7zQL0TAPiBIzUiVtBm95Hx0NOW%2BBh8tiKsTcIQVmLAz4EjwAAL0wcsCSMtex0NcACoFJjl5LLigAFn8Xfd%2BaiSnXUeYsB%2BicInVMwObTBGiWIvmlrKt6EVljaIMropOJAaX2PljERhYKsLXWqRqkyRESlUpcASJQFNf4FWABTAyU1VZ8KVrKvVjCdkBWxqGoKGzrF4HohAIZJ0cq2COoDOK7qiHsHmcqYNG9wZMDuKKjGm1lSqg2QlbSU5dIzU0QfZmT49GxO%2Bb8wFxiQVPIgkbE1tKynFrGAAnJ1w8orFQOSJc7M3AvgPPpaltLM0zOWCm9AaatnnhOgAVn8ahMyRqInP1uK/OyG83AORYr%2BbojS8DNLcF%2BY4pwrW2swJo/VOa3CFM2cvRYHw8A10JSdJEAAxTOmkrX20CmOF0dZiyNjLPiRi4ZOBFk9OweUyEX6hzfR0BuWM%2BmYYCFeYY2ZNRonCn%2BKNmAY20iquRijlGqPUZowFZd6g41MgTetYEtG2PsY42x8Q9GTogbXGBhspYQzhW3Dzdgc6YMCblAKTj1HuMMY2UiBI2bc2tVk%2BpjTcmeMbO%2BqsyCmmDOGYiNpyBSJTnUD6MOQRK5YygcnFOYcvc5aWlamRozdGFOmbivGxN7m/M0fk9%2BuKfHaxwZLE2CAImcHsA%2BF8YhsH6zSfc4FxT5G3xvhC/GKLbt9ylJOIVE86wFRGZS15qqjFujIF6BGCAmWJzThgzqWz/H7OOfuHLcMbFOVwws0OEc1IWqsdKzRurRJMDbCNAyCkkhaDHLi%2BxG49wBSSGXb0eTq1aD0GRFBUbrWEhOd6IN4znmaMztQqNsLEHhOblE3uCTQowsIeK%2Bx4blHvqFrcP5r7b2TM0fM0O3bKtThNTaJtIKrUFQWl%2B7R/7y5AerhYLdu4RIFi9G2ula6kQwcueBG5jzQWqORujUM77yXoccZyvD5riPov4B4GjxGGO8dUde7Js7NmayFkS1dyLN3ouxZeJJx7R2fsnc4x9vYTJY3RVJ%2Bp1nnHRuGmNOSCAM2OIvAW/eJbyJVvrZm%2BGSQO2muc/qw5/b7XDvFflxxirVWau7enG1u4HWOfjnYPZzrHLZ5MkHFZ2QA2rfk845IRUtBgSjey2J8dR5CtnjUy9oPHH2cXe50J3nW5LjoC3B0bS86iIMFkELxLT34/49SwFlZEvZcGet7RlP4G09RZxewHP3F2D58Lw94v1IZNabF7J8zVOawg8qFITaHQuh9Fq8bt39mjtD71EjlH9P0eYNyEoKQVuw8h4X8D2nqPV/ECh/39TlOZ92aB3iEfEQx9Y6ULYbbhvXcX4R0vunDOAJr9v2P%2B/FUAjXjVACS5jdkcThX%2Bm7CmgrSPSqTcFpkIRYmNiSjoUCh3DYEHECnTlRjOASAikClRkClpFWEwCnAKAglGkblilvkqBglORdluVSQ2ERRiGZRuh4WWA3TYIiRfGTlxRwTNhFkXnqTmjwkCXQCwE1X0VIgUE%2BhwzwwdCiUZ3qkvxYFpCr2r1o3l1hw2FG1phqhWnd2UOvzv3B1Y13yv33xX0Zy/3UA30h2OyCy0OVgRxv02jMMRymTUzcLfwPysMxyiFsLUxDx33Pxa0MIsI/yZ3sPL0l2n2UPjG7Ff3CMPx1yUFoBcOMPiivCUE5Vfn/UeFQhw181ryqjPziInEMIOkf1oCgmf1CMSJy3f2SKx2MKKMT0o0V3GxJCm1V023V2%2BDYC2kZzVl7mW1oF13ow2y20NwdzNwOyO2KPIx0PylqkRwSOpyMJ/wn0HFiIR3iOnHmLaIoxyjADACWKwIMNfwOl81kwWIo2TxCNC1Twi2tgQGHBxSL2LBL1Ywo1uN2RWXUO%2B1%2BPezWRiK8MqMNyfy8KSN8PSJ/1oGGSG0OMWIePjCV26Jmzmw1wGJeOGO1xWzWwmP122xmKdzlgOJPyo1t2qx2Opz2KnEazKPd0d3N2d16E926x90s363JIJ3I1KN2MJF0JWIuPWKuJY001%2BKqg6Im2V3WB6NoAlEwANj0LclMj2kxzGM1QwWuh4I3hAMeicVWkJM2wZIFPYCFP0LWOHzFMCKRIo1RhpJN3NOWMtIqOamey4ztL%2BM2gBK%2ByBP%2BN9L9KRN0wlx80DI0wWPr0E2ePRzeKzw%2BPgx7z8gANij0DKHBmrXCTcEaJhPYLCU3T9ScmdSdlTmEiUlcB1QgOmn0m6DFl4TODxESV/BfFqlIDVHBkhnEVBVhiqXSklmulyRgheJdj4KYJrUzkJgoMqioKXFaWoNsCpOdmQiUgxxXg4MsgyRKXUhgLQgwnlmwloUHNsB4JHJAIgHiE4T/EnBpiVMNnKmuQmmcIPlAMPBFjgP7HvVDnHJqGkn1EBT7DYjxBkNwztBzDpGLPAJBP5NpPKPqLExzM/z8PGDvxHiqJqMXMdNnwa1qNrD2wO3ZO916z9wGz/xGBvEoJVOfDOCtNVhwNYnYhQLxGAHQJgk1h1mvNAhck0SuBHQVlYFwJgnClPJ5lUSjX4gXG6GdT5XtlWFQGAGICYFNh7SASuhCRhiYGAC/XsFkNAvw0slqiUIR1pCQQBn83l2ONOJRPXD5xy2jwKwEHWARNFwcKmyZBZDIjbM22OQFmAAQEYOUNGOXU/mNK2xotOBF1r0RQpDUD0B0HYB/RMD0DMFBAlGwBF0BKD103DITxP2lK6JNHlNvlqnYjCrxECuCvUEmPDFKpUOe3l1QOYqnxmOquUPSr7zWhWSJzcAgATWyvI1Kz8kcDAotIBQdM4CspFOtPdM7CtHMVOmhE8DhB8GsGTLGGBEEVkC5VQk4R5CcOp0VmKoGIPk%2Bl9xHBwrpPPASlBDQxMgJBznGgdWfVoTwJVIwOsWvMPSwhml4sPMtiEpglLLuE%2BgBwmsjz3DsqNFjxcPvyXXsCHSOBcAgARLVCHQOFQmMNhuHhWBNCZBmzfDSCUBgTojzgjGGF1OUoUoWEyhCsGzVEIxYxOlcvlLTN4qEpprzQOoQggDFXhI5qZpm3UHRwFGlDISGmeQ2Cmxm1ptil011IZo2X5s2x/QgojGFtFooVgOcQUVcU3D1022lq%2BkxhmtinfLOBMIyCNpMFEHLm2VEGXVEFIBYDEFsAdtQDEBsL4EEESUcFoAdoIGdutvLjuBAGXVsEYDEDFQdqdtEBdtIDdtEAdp4BADDv9pjuttIDgFgCQDQD6GIzIAoHEiSl6DzpQGAEinoGmAYAXCTogASADtIASGdU3DEBHgdpzurQMHhXrqwCOF3hEFEFbtIHwHBjSTwBKSTrTtICrIrRbodvJAzUno3C8QDpttYA4C4EYESCTsgHLkYT1AnrfAMCkETsKWEDoBtrEHtsdvrvjoxAAA4oI3woIxUlBgBkBkAlBIokg0iIBcBCBM4fbwx1Ai6871QSd1A/aV7y50oI4%2BxKAg6Q6w7bbI7r7J747E7k7SBU6XaBtw7RBfbUHY70GsGoHSBZVAKnaxUgA%3D%3D) it can be seen that the 'black macros' that Exceptional consists of are completely optimized away, only keeping the stack winding/unwinding and conditional jump checking in place.


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
