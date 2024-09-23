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

//utils.c: Provides additional utilities for hxo-loader to work properly

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

#ifdef __bsd__
    #include <sys/sysctl.h>
#endif

int __attribute__((visibility("hidden"))) GetExePath(char *directory, char *exename)
{
#ifdef __bsd__
    //exe path fetch for BSD
    size_t path_size = HXO_MAX_PATH_LEN; //default size of directory struct
    int mib[4];

    // Define the MIB array for fetching the executable path
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = getpid();

    // Retrieve the executable path
    if (sysctl(mib, 4, directory, &path_size, NULL, 0) == -1) {
        //perror("sysctl");
        return 0;
    }
    /*
    char *exe_name;
    exe_name = getprogname();
    strcpy(exename, exe_name);
    */
    pid_t pid = getpid(); // Get the current process ID
    struct kinfo_proc proc_info;
    size_t size = sizeof(proc_info);

    // Use sysctl to get process information
    if (sysctl((int[]){ CTL_KERN, KERN_PROC, KERN_PROC_ALL, pid }, 4, &proc_info, &size, NULL, 0) == -1) {
        perror("sysctl");
    }

    // Print the executable name
    //printf("Executable name: %s\n", proc_info.kp_proc.p_comm);
    memcpy(exename, proc_info.kp_proc.p_comm);

#else
    //exe path fetch for linux based systems
    static const uint MAX_LENGTH = HXO_MAX_PATH_LEN;
    char *exepath = (char *)malloc(MAX_LENGTH);
    char *dir;
    char *exe_name;
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
        exe_name = basename(exepath);
        strcpy(exename, exe_name);
    }
    else
    {
        perror("readlink() error: can't fetch exe path");
        free(exepath);
        return 0;
    }
    free(exepath);
#endif
    return 1;
}

int __attribute__((visibility("hidden"))) GetPID()
{
    return (int) getpid();
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

void __attribute__((visibility("hidden"))) dircat(char *absolute, char *parent, char *child)
{
    //If child starts with a "/" take it as a absolute directory
    if(child[0] == '/') {
        //treat child as an absolute path
        strcpy(absolute, child);
    }
    else {
        //copy the parent directory first
        strcpy(absolute, parent);
        fixDIR(absolute);           //add a slash (/) is not already exists
        strcat(absolute, child);    //concat child
    }
}

int __attribute__((visibility("hidden"))) dirExists(const char *path) {
    struct stat info;

    // Use stat to get information about the path
    if (stat(path, &info) != 0) {
        // Error in accessing the path (e.g., it doesn't exist)
        return 0;
    } else if (info.st_mode & S_IFDIR) {
        // S_IFDIR bit is set, meaning it's a directory
        return 1;
    } else {
        // The path exists, but it's not a directory
        return 0;
    }
}
int __attribute__((visibility("hidden"))) fileExists(const char *filepath) {

    // Use fopen to create a handle at path
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        // Error in accessing the file (e.g., it doesn't exist)
        return 0;
    } else {
        // The path exists, but it's not a directory
        fclose(fp);
        return 1;
    }
}

int __attribute__((visibility("hidden"))) CopyFile(char *source_file, char *destination_file) {
    FILE *source = fopen(source_file, "rb");
    if (source == NULL) {
        fprintf(stderr, "[!] CopyFile failed: Could not open source file '%s'\n", source_file);
        return 1;
    }

    FILE *destination = fopen(destination_file, "wb");
    if (destination == NULL) {
        fprintf(stderr, "[!] CopyFile failed: Could not open destination file '%s'\n", destination_file);
        fclose(source);
        return 1;
    }

    char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes_read, destination) != bytes_read) {
            fprintf(stderr, "[!] CopyFile failed: Error writing to destination file '%s'\n", destination_file);
            fclose(source);
            fclose(destination);
            return 1;
        }
    }

    if (ferror(source)) {
        fprintf(stderr, "[!] CopyFile failed: Error reading from source file '%s'\n", source_file);
        fclose(source);
        fclose(destination);
        return 1;
    }

    fclose(source);
    fclose(destination);

    return 0;
}

#ifdef __ANDROID__
int __attribute__((visibility("hidden"))) getAppID(char *_ID)
{
    // Open the /proc/self/cmdline file
    FILE *f = fopen("/proc/self/cmdline", "r");
    if (f == NULL) {
        perror("Failed to open /proc/self/cmdline");
        return 1;
    }

    // Read the content of the file
    if (fgets(_ID, 512, f) != NULL) {
        // The package name is stored at the start of the file
    } else {
        perror("Failed to read package name");
        fclose(f);
        return 1;
    }

    // Close the file
    fclose(f);
    return 0;
}
int __attribute__((visibility("hidden"))) LogOutput()
{
    
    char _debugAppID[512];
    getAppID(_debugAppID);

    char outLogFile[HXO_MAX_PATH_LEN];
    strcpy(outLogFile, _LOG_DIR);
    strcat(outLogFile, _debugAppID);
    fixDIR(outLogFile);

    strcat(outLogFile, "hxo_log.txt");
    
    /* Method 1 : Failed at test
    if ( !freopen(outLogFile, "a", stdout) &&
        !freopen(outLogFile, "a", stderr) )
    {
        return 0;
    }
    */

    //Method 2: ????
    int out_fd = open(outLogFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (out_fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    // Duplicate the file descriptor to stdout and stderr
    if (dup2(out_fd, STDOUT_FILENO) == -1) {
        perror("Failed to redirect stdout");
        return 0;
    }

    if (dup2(out_fd, STDERR_FILENO) == -1) {
        perror("Failed to redirect stderr");
        return 0;
    }

    printf("\n\n\n---------->START LOG<----------\n\n");
    // Close the original file descriptors
    //close(out_fd);
    
    return out_fd;
}
#endif //__ANDROID__
