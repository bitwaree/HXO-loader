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

//hxo.h: main header file for hxo-loader

#ifndef LIC_STR
	#define LIC_STR "HXO-loader Copyright (C) 2024 bitware.\n\n"
#endif

#ifndef BANNER_STR
	#define BANNER_STR "\n\n" \
                     "          _                   \n" \
                     "    |_|\\// \\ | _  _. _| _ ._\n" \
                     "    | |/\\\\_/ |(_)(_|(_|(/_| \n" \
                     "                      --v%s  \n"
#endif //BANNER_STR
#define CPRS_SHOW_ALWAYS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

void __attribute__((visibility("hidden"))) *hxo_loader();
