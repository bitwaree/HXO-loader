//dynamic shared object loader by bitware32
//Read README.txt for config and function api defination and potential tutorial
//       _
// |_|\// \ | _  _. _| _ ._
// | |/\\_/ |(_)(_|(_|(/_|
//
#pragma once
#include <stdint.h>
#include <pthread.h>

#define NOEXPORT

#ifdef NOEXPORT
	void __attribute__((visibility("hidden"))) *hxo_loader(void*);
#else
	void *hxo_loader(void*);
#endif
