#include "exceptional.h"


// Used to alter behavour of `throw` when used outside of a `try`-block.
jmp_buf __Exceptional_env;
int __Exceptional_try_block_nesting_count = 0;
