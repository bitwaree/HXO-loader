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

//utils.h: header for utils.c
#include "config.h"

int __attribute__((visibility("hidden"))) GetExePath(char *directory);
void __attribute__((visibility("hidden"))) fixDIR(char *Dir);
void __attribute__((visibility("hidden"))) dircat(char *absolute, char *parent, char *child);
int __attribute__((visibility("hidden"))) fileExists(const char *filepath);
int __attribute__((visibility("hidden"))) dirExists(const char *path);
int __attribute__((visibility("hidden"))) CopyFile(char *source_file, char *destination_file);

#ifdef __ANDROID__
//Android specific utils
int __attribute__((visibility("hidden"))) getAppID(char *_ID);    //Fetches app id eg:<com.example.app>
int __attribute__((visibility("hidden"))) LogOutput();            //Starts the logging
#endif
