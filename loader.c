/*
HXO_LOADER source code :)
Shared object AutoLoader named as 'hxo_loader' by Bitware
copy your so files in a folder named scripts
Give your files a extention .hxo and LD_PRELOAD a so file
which will create a thread of "hxo_loader" to successfuly load the other files and execute them
NOTE: Make sure your other so files have a 'void *_init_hxo(void *)' function which will be used as the entrypoint
for your so files, make sure you start a thread execution as the caller shouldn't be kept waiting for your return,
you should focus on returning a value as fast as possible *OTHER FILES WON'T BE LOADED UNLESS YOUR ENTRY POINT RETURNS*.
*/
//  _  ___  _____    _              _
// | || \ \/ / _ \  | |___  __ _ __| |___ _ _
// | __ |>  < (_) | | / _ \/ _` / _` / -_) '_|
// |_||_/_/\_\___/  |_\___/\__,_\__,_\___|_|


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Dircetory stuff
#include <dirent.h>
// Shared dynamic lib stuff
#include <dlfcn.h>
#include "loader.h"


//#define CLOSE_LIB_AFTER_EXEC      //Uncomment if you want to unload library once the entrypoint returns

// Defined constants
#define MAX_LIBS 100
#define FILE_EXT ".hxo"
#define FILE_DIR "./scripts/"


void __attribute__((visibility("hidden"))) *hxo_loader(void *)
{
    // search for lib in the directory as per config
    DIR *dir;
    struct dirent *entry;
    char *files[MAX_LIBS]; // Assuming a maximum number of files
    int count = 0;

    dir = opendir(FILE_DIR);
    if (dir == NULL)
    {
        perror("[-] Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        { // Check if it is a regular file
            char *dot = strrchr(entry->d_name, '.');
            if (dot && strcmp(dot, FILE_EXT) == 0)
            {
                files[count] = strdup(entry->d_name);
                count++;
            }
        }
    }

    closedir(dir);

    /*
    // Print the file names stored in the array
    for (int i = 0; i < count; i++) {
        printf("%s\n", files[i]);
    }

    // Free memory allocated for strings
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
    */

    // load one by one and call their perticular entrypoint void* _init_hxo(void*)
    char current_filename[2048];
    void *handle;
    void *(*init_func)(void *);

    for (int i = 0; i < count; i++)
    {
        handle = NULL;
        // Load the shared object file
        strcpy(current_filename, FILE_DIR);
        strcat(current_filename, files[i]);
        handle = dlopen(current_filename, RTLD_LAZY);
        if (!handle)
        {
            fprintf(stderr, "[-] Error: %s\n", dlerror());
            return 1;
        }

        // Get a pointer to the _init_hxo function
        init_func = dlsym(handle, "_init_hxo");
        if (!init_func)
        {
            fprintf(stderr, "[-] Entrypoint not found in: %s\n", files[i]);
            dlclose(handle);
            return 1;
        }

        // Call the _init_hxo function
        void *result = init_func(NULL);
        if((intptr_t)result == -1)
        {
            //Error in library!!!
            fprintf(stderr, "[-] %s returned %X\n", files[i], (uintptr_t) -1);
            dlclose(handle);
        }

        // Close the shared object file
        #ifdef CLOSE_LIB_AFTER_EXEC
            dlclose(handle);
        #endif
    }

    
    // free allocated memory

    // strings
    for (int i = 0; i < count; i++)
    {
        free(files[i]);
    }

    // exit
    return 0;
}
