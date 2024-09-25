% HXO_CREATETHREAD(3) HXO Library Definitions
% bitwaree <bitware@duck.com>
% September 2024

# NAME
hxo_createthread -- Creates a thread.

# SYNOPSIS

```C
#include <hxo-user.h>              //Contains all the definitions needed to get started

int hxo_createthread(void *(*start_routine) (void *), void *arg);
```

# DESCRIPTION

`hxo_createthread()` creates a thread using the POSIX thread apis. The thread function pointer is passed through the **first argument**, followed by the **function arguements**.

# RETURN VALUES

`hxo_createthread()` returns a 0, if succeeds. On error, it returns an error number. _(please consider having a look at `pthread_create(3)`)._

# EXPANDS TO

```C
#include <pthread.h>
#define HXO_THREAD_FUNCTION void *(*) (void*)

inline int hxo_createthread(void *(*start_routine) (void *), void *arg)
{
    pthread_t thr = 0;
    return pthread_create(&thr, 0, start_routine, arg);
}
```

# SEE ALSO
hxo-loader(1) -- general guide  
hxo-devl(3) -- hxo module development guide  
hxo_cmpversion(3) -- compares semantic versions
