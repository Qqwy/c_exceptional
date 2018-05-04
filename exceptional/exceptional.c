#include "exceptional.h"


// Buffer to keep exception throwing state.
jmp_buf __Exceptional_env;
// Used to alter behavour of `throw` when used outside of a `try`-block.
int __Exceptional_try_block_nesting_count = 0;
