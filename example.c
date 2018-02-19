#include <stdio.h>


#define try while(1)                                          \
    if(0){                                                    \
    finished:                                                 \
      break;                                                  \
    } else                                                    \
      for(int switcher = 0;;)                                 \
        while(1)                                              \
          if(switcher)                                        \
            goto finished;                                    \
          else                                                \
            for(switcher = 0; switcher <= 2; ++switcher)      \
              if(switcher == 0)                               \

#define catch(exception) else                                           \
    for(int exception = switcher;switcher <= 2; ++switcher)             \
      if(switcher == 1)                                                 \


#define finally else

int main(){
  int foo = 0;
  try {
    foo += 1;
    printf("foo try: %d\n", foo);
    } catch(err) {
    printf("foo catch: %d, err: %d\n", foo, err);
    } finally {
    printf("Foo finally %d\n", foo);
    }

  return 0;
}
