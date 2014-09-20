#include "statTracker.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>


Tracker::Tracker()
{
    //pthread_t timmerThread;
    //pthread_create(&timmerThread, NULL, &timer, NULL);
    
}

void Tracker::load(){
    using namespace boost::interprocess;
    
    //Open already created shared memory object.
    shared_memory_object shm (open_only, "MySharedMemory", read_only);
    
    //Map the whole shared memory in this process
    mapped_region region(shm, read_only);

    data = *((Data *) region.get_address());
}

void Tracker::init(){
    using namespace boost::interprocess;

    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
    } remover;
    
    //Create a shared memory object.
    shared_memory_object shm (create_only, "MySharedMemory", read_write);
    
    
    //Set size
    shm.truncate(sizeof(data));
    
    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);
    
    data = *((Data *) region.get_address());
    
    //Write all the data to shared memory.
    //std::memcpy(region.get_address(), &data, region.get_size());
}