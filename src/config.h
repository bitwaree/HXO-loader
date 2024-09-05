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

//config.h: tweakble configs

#define DEFAULT_HXO_DIR "./scripts/"
//#define DEFAULT_LIB_DIR "/usr/lib"
#define CONFIGFILE "HXO.ini"

#ifdef __ANDROID__
    //In case of android
    #define _DEBUG_LOG            //Comment if you don't wanna store logs in a file
    #define DEFAULT_ANDROID_APPPATH "/storage/emulated/0/Android/media/"        //Parent directory of <id>/scripts
    #define _LOG_DIR DEFAULT_ANDROID_APPPATH                           //Where hxo_log.txt file will be stored
    //#define DEFAULT_INI_DIR "/storage/emulated/0/hxo/"                //where HXO.ini will be stored
    #define DEFAULT_LIB_DIR "/storage/emulated/0/hxo/"                //Where hxo_loader.so is placed
    
    int __attribute__((visibility("hidden"))) getAppID(char *_ID);    //Fetches app id eg:<com.example.app>
    int __attribute__((visibility("hidden"))) LogOutput();            //Starts the logging
#else
    //in case of non-android systems
    #define DEFAULT_LIB_DIR "/usr/lib"
#endif //__ANDROID__
