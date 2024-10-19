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

#define DEFAULT_HXO_DIR "./modules/"
//#define DEFAULT_LIB_DIR "/usr/lib"
#define CONFIGFILE "HXO.ini"

#ifdef __ANDROID__
    //In case of android
    #define _DEBUG_LOG            //Comment if you don't wanna store logs in a file
    #define DEFAULT_ANDROID_APPPATH "/storage/emulated/0/Android/media/"        //Parent directory of <id>/scripts
    #define _LOG_DIR DEFAULT_ANDROID_APPPATH                           //Where hxo_log.txt file will be stored
    //#define DEFAULT_INI_DIR "/storage/emulated/0/hxo/"                //where HXO.ini will be stored
    #define DEFAULT_LIB_DIR "/storage/emulated/0/hxo/"                //Where hxo_loader.so is placed
#else
    //in case of non-android systems
    #define DEFAULT_LIB_DIR "/usr/lib"
#endif //__ANDROID__

#define MAX_LIBS 100                //Set maximum num of hxo file injectable
#define HXO_MAX_PATH_LEN 4096           //Set maximum absolute path length
#define HXO_MAX_FILE_LEN 512            //Set maximum file name length
