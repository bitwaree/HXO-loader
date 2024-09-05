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

//hxo.c: contains main code responsible for so loading

#include "hxo.h"
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include "loader.h"
#include "ini.h"
#include "utils.h"
#include "config.h"
// Defined constants

struct iniParam
{
    bool Enable;          //Enables hxo_loader
    char hxo_dir[2048];   //Child directory name (where .hxo files stays)
    int sleep;            //Sleep timer in seconds
    bool autoUnload;      //Unloads hxo files after execution
    //extras
    char ep[256];         //Entrypoint name
    char dl_dir[2048];    //where hxo_loader stays
    //misc
    bool hideBanner;      //Hide banner text
    bool hideCPRstring;   //Hide Copyright String
};

struct internalParam
{
    char exedir[2048];    //(Absolute path) Where elf executable stays
    char cwd[2048];       //(Absolute path) Where elf was executed
    char ini_dir[2048];   //(Absolute path) Where INI file was loaded (parent directory for hxo_dir) (Absolute Path)
    char iniFile[2048];   //(Absolute path) Path of iniFile 
    char hxo_dir[2048];   //(Absolute path) Where .hxo files lives 
};

#ifdef __ANDROID__
struct AndroidParam
{
    char ID[512];                   //App ID
    char AndroidDataPath[1024];     //equvalent to /sdcard/data/<id>/
    char rootDataPath[1024];        //equvalent to /data/data/<id>/
};
#endif

#define MAX_LIBS 100                //Set maximum num of hxo file injectable
#define FILE_EXT ".hxo"

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
    else if(!strcmp(section, "MISC"))
    {
        //misc
        if(!strcmp(name, "HideBanner"))
            cf->hideBanner = (bool) atoi(value);
        if(!strcmp(name, "HideCPRS"))
            cf->hideCPRstring = (bool) atoi(value);
    }
    else
    {
        return 0;
    }

    return 1;
}


void __attribute__((visibility("hidden"))) *hxo_loader()
{
#ifdef __ANDROID__
int out_fd = 0;
#ifdef _DEBUG_LOG
    out_fd = LogOutput();    //Start the debug log on android
#endif
#endif

#ifdef CPRS_SHOW_ALWAYS
    fprintf(stdout, BANNER_STR, VER_STR);
    fprintf(stdout, LIC_STR);
#endif

#if !defined(__ANDROID__) || defined(__STD_UNIX___)
    //On standard OS... (unix/linux, excluding android)
    //Read Config
    struct internalParam *entParam = malloc(sizeof(struct internalParam));
    
    struct iniParam *confparam = malloc(sizeof(struct iniParam));
    confparam->Enable = 1;
    confparam->sleep = 0;
    strcpy(confparam->dl_dir, DEFAULT_LIB_DIR);
    strcpy(confparam->hxo_dir, DEFAULT_HXO_DIR);
    strcpy(confparam->ep, "_init_hxo");
    confparam->autoUnload = 0;
    confparam->hideBanner = 0;
    confparam->hideCPRstring = 0;


    // fetch current working directory
    if (getcwd(entParam->cwd, 2048) == NULL) 
    {
        perror("[!] WARNING: Can't retrive current working directory!");
        *entParam->cwd = (unsigned char) 0;
    }
    //fetch exe path
    if(!GetExePath(entParam->exedir))
    {
        perror("[X] ERROR: Can't retrive current executable path! \n");
        *entParam->exedir = (unsigned char) 0;
    }

    //INI persing priority:
    //FIRST: look for current woking directory
    //SEC:   look for Executable path
    if(*entParam->cwd == 0 && *entParam->exedir==0)
    {
      _exit_at_init:
        //If nothing found... (atleast one is needed to continue)
        free(entParam);
        free(confparam);
        return (void*)1;
    }

    if(*entParam->cwd != 0)
    {
        dircat(entParam->iniFile, entParam->cwd, CONFIGFILE);
        if(!(ini_parse(entParam->iniFile, fn_ini_handler, confparam) < 0))
        {
            //Successfully parsed ini from current working directory
            strcpy(entParam->ini_dir, entParam->cwd);
            fixDIR(entParam->ini_dir);
            goto after_parsing;
        }
    }
    
    if (*entParam->exedir != 0) 
    {
        dircat(entParam->iniFile, entParam->exedir, CONFIGFILE);
        if(!(ini_parse(entParam->iniFile, fn_ini_handler, confparam) < 0))
        {
            //Successfully parsed ini from executable directory
            strcpy(entParam->ini_dir, entParam->exedir);
            fixDIR(entParam->ini_dir);
            goto after_parsing;
        }
        else
        {
            //Assume all parameter as per default values
            perror("[!] WARNING: unable to parse \'HXO.ini\'");
            strcpy(entParam->ini_dir, entParam->exedir);
            fixDIR(entParam->ini_dir);
            goto after_parsing;
        }
    }
    else
    {
        goto _exit_at_init;
    }

  after_parsing:

    //exit without ding anythig if config says to
    if(!confparam->Enable)
    {
        free(entParam);
        free(confparam);
        return (void*)1;
    }
    //setup parameters
    dircat(entParam->hxo_dir, entParam->ini_dir, confparam->hxo_dir);
    //Add a slash to avoid directory issues
    fixDIR(entParam->hxo_dir);

#ifndef CPRS_SHOW_ALWAYS
    if(!confparam->hideBanner)
        fprintf(stdout, BANNER_STR, VER_STR);
    if(!confparam->hideCPRstring)
        fprintf(stdout, LIC_STR);
#endif //CPRS_SHOW_ALWAYS

    // search for lib in the directory as per config
    DIR *dir;
    struct dirent *entry;
    char *files[MAX_LIBS]; // Assuming a maximum number of files
    int count = 0;

    dir = opendir(entParam->hxo_dir);
    if (dir == NULL)
    {
        fprintf(stderr, "[X] ERROR: Can't open hxo directory \"%s\".\n", confparam->hxo_dir);
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

    char current_filename[2048];
    void *dlhandle;
    void *(*init_func)(void *);
#else //IN CASE OF ANDROID
    //Read Config
    struct internalParam *entParam = malloc(sizeof(struct internalParam));
    
    struct iniParam *confparam = malloc(sizeof(struct iniParam));
    confparam->Enable = 1;
    confparam->sleep = 0;
    strcpy(confparam->dl_dir, DEFAULT_LIB_DIR);
    strcpy(confparam->hxo_dir, DEFAULT_HXO_DIR);
    strcpy(confparam->ep, "_init_hxo");
    confparam->autoUnload = 0;
    confparam->hideBanner = 0;
    confparam->hideCPRstring = 0;

    //in case of android
    struct AndroidParam *androidParam = malloc(sizeof(struct AndroidParam));

    //Default parameters
    if(getAppID(androidParam->ID))
    {
        fprintf(stderr,"Unable to fetch ID...");
    #ifdef _DEBUG_LOG
        fflush(stdout);
        fflush(stderr);
        close(out_fd);
    #endif
        free(entParam);
        free(confparam);
        free(androidParam);
        return (void*)1;
    }
    printf("[+] Got ID: %s\n", androidParam->ID);

    //Setup additional android parameters
    dircat(androidParam->rootDataPath, "/data/data/", androidParam->ID);
    dircat(androidParam->AndroidDataPath, DEFAULT_ANDROID_APPPATH, androidParam->ID);
    printf("\nandroidParam->rootDataPath: %s\nandroidParam->AndroidDataPath: %s\n", androidParam->rootDataPath, androidParam->AndroidDataPath);
    //Setting up parameters for loading HXO.ini
    strcpy(entParam->ini_dir, androidParam->AndroidDataPath);
    fixDIR(entParam->ini_dir);
    dircat(entParam->iniFile, entParam->ini_dir, CONFIGFILE);
    
    
    if(!dirExists(androidParam->AndroidDataPath))
    {
        if (mkdir(androidParam->AndroidDataPath, 0770) == -1)
        {
            if(errno != EEXIST)
            {
                fprintf(stderr, "[X] Can't create directory: %s\n", androidParam->AndroidDataPath);
                // free allocated memory           
                free(confparam);
                free(entParam);
                free(androidParam);
            // exit
            #ifdef _DEBUG_LOG
                fflush(stdout);
                fflush(stderr);
                if(out_fd != 0) {
                    close(out_fd);
                }
            #endif
                return (void*)1;
            }
        } else {
            if(!out_fd)
            {
                out_fd = LogOutput();
                fflush(stdout);
                fflush(stderr);
            }
            printf("[!+] Directory not found, created one: %s\n", androidParam->AndroidDataPath);
        }
    }


    //Parse ini file
    if(ini_parse(entParam->iniFile, fn_ini_handler, confparam) < 0)
    {
        perror("[!] WARNING: unable to parse \'HXO.ini\'");
    }
  after_parsing:

    //exit without ding anythig if config says to
    if(!confparam->Enable)
    {
        free(entParam);
        free(confparam);
        free(androidParam);
    #ifdef _DEBUG_LOG
        fflush(stdout);
        fflush(stderr);
        close(out_fd);
    #endif
        return (void*)1;
    }
    //setup parameters
    //HXO Priority of searching for 
    dircat(entParam->hxo_dir, androidParam->AndroidDataPath, confparam->hxo_dir);
    char new_hxo_dir[512];
    dircat(new_hxo_dir, androidParam->rootDataPath, "cache/hxo/");
    //Add a slash to avoid directory issues
    fixDIR(entParam->hxo_dir);
    fixDIR(new_hxo_dir);
    

#ifndef CPRS_SHOW_ALWAYS
    if(!confparam->hideBanner)
        fprintf(stdout, BANNER_STR, VER_STR);
    if(!confparam->hideCPRstring)
        fprintf(stdout, LIC_STR);
#endif //CPRS_SHOW_ALWAYS

    // search for lib in the directory as per config
    DIR *dir;
    struct dirent *entry;
    char *files[MAX_LIBS]; // Assuming a maximum number of files
    int count = 0;

    dir = opendir(entParam->hxo_dir);
    if (dir == NULL)
    {
        fprintf(stderr, "[X] ERROR: Can't open hxo directory \"%s\".\n", confparam->hxo_dir);
        free(entParam);
        free(confparam);
        free(androidParam);
    #ifdef _DEBUG_LOG
        fflush(stdout);
        fflush(stderr);
        close(out_fd);
    #endif
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

    // proceed to copy files to the rootDataPath/cache/hxo/
    char current_filename[2048];
    void *dlhandle;
    void *(*init_func)(void *);

    //copy files to the tmp folder
    //folder: /data/data/<APP_ID>/tmp/hxo/
    if (mkdir(new_hxo_dir, 0777) == -1)
    {
        if(errno != EEXIST)
        {
            fprintf(stderr, "[X] Can't create directory: %s", new_hxo_dir);
            // free allocated memory
            for (int i = 0; i < count; i++)
            {
                free(files[i]);
            }            
            free(confparam);
            free(entParam);
            free(androidParam);
        // exit
        #ifdef _DEBUG_LOG
            fflush(stdout);
            fflush(stderr);
            close(out_fd);
        #endif
            return (void*)1;
        }
    }

    char new_filename[2048];
    for (int i = 0; i < count; i++)
    {
        dircat(current_filename, entParam->hxo_dir, files[i]);
        dircat(new_filename, new_hxo_dir, files[i]);
        if(CopyFile(current_filename, new_filename))
        {
            // free allocated memory
            for (int i = 0; i < count; i++)
            {
                free(files[i]);
            }            
            free(confparam);
            free(entParam);
            free(androidParam);
        // exit
        #ifdef _DEBUG_LOG
            fflush(stdout);
            fflush(stderr);
            close(out_fd);
        #endif
            return (void*)1;
        }
    }
    strcpy(entParam->hxo_dir, new_hxo_dir);
#endif //__ANDROID__

    if(confparam->sleep > 0)
    {
        //Sleep before loading
        sleep(confparam->sleep);
    }
    // load one by one and call their perticular entrypoint void* _init_hxo(void*)
    // char current_filename[2048];
    // void *dlhandle;
    // void *(*init_func)(void *);

    for (int i = 0; i < count; i++)
    {
        dlhandle = NULL;
        // Load the shared object file
        dircat(current_filename, entParam->hxo_dir, files[i]);
        dlhandle = dlopen(current_filename, RTLD_LAZY);
        if (!dlhandle)
        {
            fprintf(stderr, "[!] Error while loading %s: %s\n", current_filename, dlerror());
            continue;
        }

        // Get a pointer to the _init_hxo function
        init_func = dlsym(dlhandle, confparam->ep);
        if (!init_func)
        {
            fprintf(stderr, "[!] Entrypoint not found in: %s\n", files[i]);
            dlclose(dlhandle);
            continue;
        }

        // Call the _init_hxo function
        void *result = init_func(NULL);
        //If a non-zero value returned...
        if((intptr_t)result != 0)
        {
            //Print about the error value
            fprintf(stderr, "[*] \"%s\" returned %ld (0x%lX).\n", files[i], (int64_t) result, (uintptr_t) result);
        }
        if((intptr_t)result == -1)
        {
            //Error in library!!!
            dlclose(dlhandle);
            continue;
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
#ifdef __ANDROID__
    free(androidParam);
#ifdef _DEBUG_LOG
    fflush(stdout);
    fflush(stderr);
    close(out_fd);
#endif
#endif
    // exit
    return (void*)0;
}
