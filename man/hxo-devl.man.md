% HXO-DEVL(3) Library definations and Demo
% bitwaree <bitware@duck.com>
% September 2024

# NAME
hxo-devl -- Hybrid Xpl0it module programming demo.

# SYNOPSIS

```C
#include <hxo-core.h>              //Contains all the definations needed to get started

size_t _init_hxo(struct HXOParam *hxoParams)    //HXO predefined entrypoint
{

    //..... YOUR INIT CODE HERE

    if (/*..ERROR AR INIT..*/)
    {
        return -1;                 //for FETAL error(s)
    }

    if (/*..MINOR ERRORS..*/)
    {
        return /*error_value*/;   //Not a fully successful initialize but isn't fetal
                                  //Can be used for debug purposes.
    }    
    return 0;                     //for Successful Initialization :)
}
```

# DESCRIPTION
This page demonstrates all nessesary things to know to create Hybrid Xpl0it loader (HXO-loader) compatible (.hxo) modules.


# USAGE

- "`_init_hxo`" is the **default _entrypoint_**, which is called the hxo module is loaded. `_init_hxo` has a pointer to a `struct HXOParam` as a parameter. The struct can be _ignored_ but acctually it can have a lot of use cases. Such as: 

    1. To check if the module is injected into the correct process;
    2. To get it's own name and absolute path, which can be used for loading resourcesand stuffs, etc.

  The `struct HXOParam` Defination

    ```C
    struct HXOParam {
        char *baseName;         //Containing the elf executable name
                                //or, In case of Android: The APP ID

        char *basePath;         //elf executable path
        char *moduleName;       //The hxo module's file name 
        char *modulePath;       //the hxo module's absolute path
        //char *
    };
    ```

- `_init_hxo` have a return type of `void *`, which is used for the module handling, different return values result different action on the module. For example:

    1. `return (void*)  0`    : means initialization successful, module will stay loaded unless `AutoUnloadAfterExecution` is explicitly enabled.
    2. `return (void*) -1`    : means **FETAL ERROR** at initialization, module will be unloaded after `_init_hxo` returns.
    3. **Any other _value_**  : means successful injection but with warnings. The returned value will be displayed in hxo log. This is used for debugging. This module behaves same as return value `0`.

  \* As `size_t` is same as `void *`, using size_t as the return type can be helpful.

- ***WARNING***: Here are a few things to keep in mind if you don't wanna encounter a **seg-fault**.. : )

    1. If a _continious loop_ processing in needed for a specific module, start a thread using `pthread_create`, **do not** create an infinite loop under the `_init_hxo` function, otherwise, HXO-loader will be frozen and other hxo modules will not get a chance of getting loaded. Always try to run the nessesary checks, initialization code and return a proper value as fast as possible.
    2. In case of **fetal error** _(return value -1)_, it must be made sure there is ***no thread running*** in that module. If a thread runs in the module while `_init_hxo` returns `-1`, it will cause a _segmentation fault_.
    3. If `AutoUnloadAfterExecution` is turned ON in `HXO.ini` config, all the modules in that case have to follow one simple rule: _"Do everything is the init function **No Running Thread** after _hxo_init returns"_.
    4. The `sleep` timer counts a timer for the hxo files to be loaded. By default HXO.ini has `sleep = 0` but it may cause issues related to ***too early injection***. It's advised to modify it accordingly to give the process some time to let it's job done, before the injection proceeds. For example, use a `sleep = 10` as a 10 second delay before the injection proceeds.



# DEMO SNIPPET

- For C:

```C
#include <hxo-core.h>              //Contains all the definations needed to get started

size_t _init_hxo(struct HXOParam *hxoParams)    //HXO predefined entrypoint
{

    //..... YOUR INIT CODE HERE

    if (/*..ERROR AR INIT..*/)
    {
        return -1;                 //for FETAL error(s)
    }

    if (/*..MINOR ERRORS..*/)
    {
        return /*error_value*/;   //Not a fully successful initialize but isn't fetal
                                  //Can be used for debug purposes.
    }    
    return 0;                     //for Successful Initialization :)
}
```

- For C++:

```C++
extern "C" {
#include <hxo-core.h>              //Contains all the definations needed to get started

size_t _init_hxo(struct HXOParam *hxoParams)    //HXO predefined entrypoint
{

    //..... YOUR INIT CODE HERE

    if (/*..ERROR AR INIT..*/)
    {
        return -1;                 //for FETAL error(s)
    }

    if (/*..MINOR ERRORS..*/)
    {
        return /*error_value*/;   //Not a fully successful initialize but isn't fetal
                                  //Can be used for debug purposes.
    }    
    return 0;                     //for Successful Initialization :)
}
}
```

# SEE ALSO
hxo-loader(1) - general guide
