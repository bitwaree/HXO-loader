//Demo of hxo_loader by bitware
#include "loader.h"

//I am overriding memset function here :)
void *memset(void* s, int c, size_t n) 
{
        //loader code start
	static int callnum = 0;
        if(!callnum)
        {
                callnum = ~callnum;
                pthread_t loader_thread;
                pthread_create(&loader_thread, 0, (void*(*)(void*))hxo_loader, 0);
        }
        //Loader code end
	//memset default code
	for (unsigned int i = 0;i < n; i++)
	{
		((unsigned char*) s)[i] = (unsigned char) c;
	}
	return s;
}
