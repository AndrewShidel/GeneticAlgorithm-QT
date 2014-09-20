#include "tracker.h"

#include <unistd.h>
#include <sys/stat.h>

Tracker::Tracker()
{
}

void load(){
    int sharedMemoryPipe = open("_GE_Shared_Memory", O_WRONLY);
    read(sharedMemoryPipe, &data, 8);
}
