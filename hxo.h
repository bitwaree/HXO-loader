/*-----------(START OF LICENSE NOTICE)-----------*/
/*
 * This file is part of HXO-loader.
 *
 * HXO-loader is licensed under the GNU General Public License v3.0
 * (GPL-3.0). You may copy, modify, and distribute it under the terms of the
 * GPL-3.0. This software is distributed WITHOUT ANY WARRANTY; see the GPL-3.0
 * for more details. You should have received a copy of the GPL-3.0 along
 * with this software. If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024 bitware
*/
/*-----------(END OF LICENSE NOTICE)-----------*/

//dynamic shared object loader by bitware
//Read README.md for config and function api defination and potential tutorial
//       _
// |_|\// \ | _  _. _| _ ._
// | |/\\_/ |(_)(_|(_|(/_|
//
//patchelf --add-needed /lib/hxo/hxo_loader.so <executable_target.elf>

#define LIC_STR "HXO-loader Copyright (C) 2024 bitware.\n\n"

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

void __attribute__((visibility("hidden"))) *hxo_loader(void *);
int __attribute__((visibility("hidden"))) GetExePath(char *directory);
void __attribute__((visibility("hidden"))) fixDIR(char *Dir);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
//#include "loader.h"
#include "ini.h"
#include <libgen.h>

// Defined constants
#define MAX_LIBS 100
#define FILE_EXT ".hxo"
#define FILE_DIR "./scripts/"
#define CONFIGFILE "HXO.ini"

struct iniParam
{
    bool Enable;
    char hxo_dir[2048];
    int sleep;
    bool autoUnload;
    //extras
    char ep[256];
    char dl_dir[2048];
};

struct enternalParam
{
    char exedir[2048];
    char cwd[2048];
    char iniFile[2048];
    char hxo_dir[2048];
};
/*
;INI struct
[HXO]
hxo=1
hxo_dir=./scripts
sleep=0
UnloadAfterExecution=0

[1337]
lib=/usr/lib/hxo
EP=_init_hxo
*/

//hook snippet
/*
    //loader code start
    static int callnum = 0;
    if(!callnum)
    {
        callnum = ~callnum;
        pthread_attr_t ptattr;
        size_t _stacksize = 4 * 1024 * 1024; // 4MB stack size

        pthread_attr_init(&ptattr);
        pthread_attr_setstacksize(&ptattr, _stacksize);
        pthread_t loader_thread;
        pthread_create(&loader_thread, &ptattr, (void*(*)(void*))hxo_loader, 0);
    }
*/
int __attribute__((visibility("hidden"))) fn_ini_handler(void *user, const char *section, const char *name, const char *value)
{

    struct iniParam *cf = (struct iniParam*) user;
    if(!strcmp(section, "HXO"))
    {
        //HXO configs
        if(!strcmp(name, "hxo"))
            cf->Enable = atoi(value);
        if(!strcmp(name, "hxo_dir"))
            strcpy(cf->hxo_dir, value);
        if(!strcmp(name, "sleep"))
            cf->sleep = atoi(value);
        if(!strcmp(name, "UnloadAfterExecution"))
            cf->autoUnload = atoi(value);
    }
    else if(!strcmp(section, "1337"))
    {
        //extra hacks
        if(!strcmp(name, "EP"))
            strcpy(cf->ep, value);
        if(!strcmp(name, "lib"))
            strcpy(cf->dl_dir, value);
    }
    else
    {
        return 0;
    }

    return 1;
}


void __attribute__((visibility("hidden"))) *hxo_loader(void *)
{

    fprintf(stdout, LIC_STR);
    //Read Config
    struct enternalParam *entParam = malloc(sizeof(struct enternalParam));
    
    struct iniParam *confparam = malloc(sizeof(struct iniParam));
    confparam->Enable = 1;
    confparam->sleep = 0;
    strcpy(confparam->dl_dir, "/usr/lib/hxo");
    strcpy(confparam->hxo_dir, "./scripts/");
    strcpy(confparam->ep, "_init_hxo");
    confparam->autoUnload = 0;

    
    if(!GetExePath(entParam->exedir))
    {
        perror("Can't retrive current executable path! Exiting!");
        free(entParam);
        free(confparam);
        return (void*)1;
    }
    else
    {
        strcpy(entParam->iniFile, entParam->exedir);
        strcat(entParam->iniFile, CONFIGFILE);
    }

    if (ini_parse(entParam->iniFile, fn_ini_handler, confparam) < 0) {
        perror("Can't load 'HXO.ini'\n");
    }
    //exit without ding anythig if config says to
    if(!confparam->Enable)
    {
        free(entParam);
        free(confparam);
        return (void*)1;
    }
    //setup parameters
    strcpy(entParam->hxo_dir, entParam->exedir);
    strcat(entParam->hxo_dir, confparam->hxo_dir);
    //Add a slash to avoid directory issues
    fixDIR(entParam->hxo_dir);

    // search for lib in the directory as per config
    DIR *dir;
    struct dirent *entry;
    char *files[MAX_LIBS]; // Assuming a maximum number of files
    int count = 0;

    dir = opendir(entParam->hxo_dir);
    if (dir == NULL)
    {
        perror("[-] Error opening directory");
        free(entParam);
        free(confparam);
        return (void*)1;
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


    if(confparam->sleep > 0)
    {
        //Sleep before loading
        sleep(confparam->sleep);
    }
    // load one by one and call their perticular entrypoint void* _init_hxo(void*)
    char current_filename[2048];
    void *dlhandle;
    void *(*init_func)(void *);

    for (int i = 0; i < count; i++)
    {
        dlhandle = NULL;
        // Load the shared object file
        strcpy(current_filename, entParam->hxo_dir);
        strcat(current_filename, files[i]);
        dlhandle = dlopen(current_filename, RTLD_LAZY);
        if (!dlhandle)
        {
            fprintf(stderr, "[-] Error while opening %s: %s\n", current_filename, dlerror());
        }

        // Get a pointer to the _init_hxo function
        init_func = dlsym(dlhandle, confparam->ep);
        if (!init_func)
        {
            fprintf(stderr, "[-] Entrypoint not found in: %s\n", files[i]);
            dlclose(dlhandle);
        }

        // Call the _init_hxo function
        void *result = init_func(NULL);
        if((intptr_t)result == -1)
        {
            //Error in library!!!
            //fprintf(stderr, "[-] %s returned %X\n", files[i], (uintptr_t) -1);
            dlclose(dlhandle);
        }
        if(confparam->autoUnload)
        {
            dlclose(dlhandle);
        }
    }

    
    // free allocated memory

    // strings
    for (int i = 0; i < count; i++)
    {
        free(files[i]);
    }
    free(confparam);
    free(entParam);
    // exit
    return (void*)0;
}

int __attribute__((visibility("hidden"))) GetExePath(char *directory)
{
    static const uint MAX_LENGTH = 1024;
    char *exepath = (char *)malloc(MAX_LENGTH);
    char *dir;
    ssize_t len = readlink("/proc/self/exe", exepath, MAX_LENGTH - 1);
    if (len != -1)
    {
        exepath[len] = '\0';
        // printf("exe path: %s\n", exepath);
        dir = dirname(exepath);
        // printf("Current directory: %s\n", dir);
        strcpy(directory, dir);
        size_t dirlen = strlen(dir);
        directory[dirlen] = '/';
        directory[dirlen + 1] = '\0';
        dirlen++;
    }
    else
    {
        perror("readlink() error: can't fetch exe path");
        free(exepath);
        return 0;
    }
    free(exepath);
    return 1;
}

void __attribute__((visibility("hidden"))) fixDIR(char *Dir)
{
    size_t tmp_length = strlen(Dir);
    if(Dir[tmp_length-1] != '/')
    {
        Dir[tmp_length] = '/';
        Dir[tmp_length+1] = '\0';
    }
}
