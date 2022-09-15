#include <iostream>
#include <fstream>

#include "storage_new.h"

using namespace std; 

Storage::Storage(int disk_size, int block_size){
    this->disk_size = disk_size;
    this->block_size = block_size; 
    this->total_blocks = disk_size/block_size; 
    this->block_size_used = 0; 
    this->internal_block_size_used = 0;
    this->actual_size_used = 0;
    this->allocated_blocks = 0;
    this->block_accessed = 0; 
    this->block_id = 0;
}

/*
    Get functions
*/
int Storage::get_disk_size(){
    return disk_size;
}

int Storage::get_block_size(){
    return block_size;
}

int Storage::get_total_blocks(){
    return total_blocks;
}

int Storage::get_block_size_used(){
    return block_size_used;
}

int Storage::get_internal_block_size_used(){
    return internal_block_size_used;
}

int Storage::get_actual_size_used(){
    return actual_size_used;
}

int Storage::get_block_id(){
    return block_id;
}


/*
    Check if adding a new block exceeds total disk space.
    Returns true if there is available space, and updates variables to add new block
    Returns false if there is no available space.  
*/
bool Storage::check_new_block(){
    if ((block_size + block_size_used) <= disk_size)  
    {
        // Update total block size used
        block_size_used += block_size;
        block_id = allocated_blocks * block_size;
        // Reset internal block size used to 0 as it is a new block 
        internal_block_size_used = 0;
        // Increase allocated blocks 
        allocated_blocks += 1;

        return true;
    }
    else
    {
        return false;
    }
}

/*
    Return an allocated block ID to insert into 
*/
reviewAddress Storage::record_get_block_add(int record_size){
    if (record_size > block_size){
        // throw some error
    }
    
    // if current block not allocated or record exceeds size after adding to existing block, make new block
    if ( (allocated_blocks == 0) || ((internal_block_size_used + record_size) > block_size)){
        bool isSuccessful = check_new_block(); 
        if (isSuccessful == false){
            // throw error
        }
    }
    // else no variables are reset, just continue flow

    // set offset to where record will be inserted 
    int offset = internal_block_size_used; 
    // increase internal size 
    internal_block_size_used += record_size; 
    // increase actual size 
    actual_size_used += record_size; 

    // return the address and offset of record  
    reviewAddress ra = {block_id, offset};
    return ra; 
    
}

/*
    Given the address of the block and size, retrieve the actual record
*/
reviewRecord Storage::retrieve_record(reviewAddress ra, int record_size){
    try {
        // get source add
        void *source_add = (int *)ra.block_add+ra.offset;

        void *main_mem_add; 

        // void * memcpy ( void * destination, const void * source, size_t num );
        // copy to mem
        memcpy(main_mem_add, source_add, record_size);

        // increase no. of blocks accessed
        block_accessed++; 

        // return record
   }
   catch(...){
        // throw some error msg 
        
   }

}

/*
    Given address of data, pointer to record and record size, insert record 
*/
bool Storage::insert_record(reviewAddress ra, void *record, int record_size){
   try {
        // get destination add
        void *dest_add = (int *)ra.block_add+ra.offset;

        // void * memcpy ( void * destination, const void * source, size_t num );
        // copy to mem
        memcpy(dest_add, record, record_size);

        // increase no. of blocks accessed
        block_accessed++; 

        return true; 
   }
   catch(...){
        // throw some error msg 
        return false; 
   }
    
}

/*
    Remove records
*/

bool Storage::remove_record(reviewAddress ra, int record_size){
    try {
        // get destination add
        void *dest_add = (int *)ra.block_add+ra.offset;

        // void * memcpy ( void * destination, const void * source, size_t num );
        // delete record by putting \0 
        memcpy(dest_add, '\0', record_size);

        // update size used
        actual_size_used -= record_size; 

        // check if block empty 
        // create empty block 
        // Note: Might have an issue here but I can't debug right now will fix !!
        char check_block[block_size];
        memset(check_block, '\0', block_size); 

        // compare 2 blocks, if original one equals empty block, means empty. 
        if (memcmp(check_block, (int *)ra.block_add, block_size) == 0){
            block_size_used -= block_size; 
            allocated_blocks --; 
        }
       
        // increase no. of blocks accessed
        block_accessed++; 

        return true; 
   }
   catch(...){
        // throw some error msg 
        return false; 
   } 
}