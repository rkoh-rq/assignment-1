#include <iostream>
#include <fstream>
// using namespace std; 

struct reviewRecord{
    char t_const [11];
    float avg_rating;
    int num_votes; 
};

struct reviewAddress {
    int block_add; // block it is in
    short int offset; // from which part of memory in block it started 
};

struct reviewChain
{
    reviewAddress reviewAddr;
    reviewChain* next;
};

class Storage {
    public:
        Storage(std::size_t disk_size, std::size_t block_size); 
        
        // helper functions
        bool check_new_block(); // check if current block_size_used + block_size is more than disk_size
        reviewAddress record_get_block_add(std::size_t record_size); // returns block add to store to 
        // CRUD functions, without update 
        reviewRecord retrieve_record(reviewAddress ra, std::size_t record_size); // retrieve record given reviewAddress and record size
        bool insert_record(reviewAddress ra, reviewRecord record, std::size_t record_size); // store record
        bool remove_record(reviewAddress ra, std::size_t record_size); // remove record, return false if error
        int reset_blocks(); 
        // get functions 
        int get_disk_size();
        int get_block_size(); 
        int get_total_blocks();
        int get_block_size_used();
        int get_internal_block_size_used(); 
        int get_actual_size_used();
        int get_block_id();
        int get_allocated_blocks();

    private:
        // Disk 
        int disk_size; // total space in disk 

        // Block
        int block_size; // size of each block 
        int total_blocks; // total number of blocks based on block size 
        int block_size_used; // total block size used
        int internal_block_size_used; // in current block, size used
        int actual_size_used; // actual size used 
        int allocated_blocks; // number of allocated blocks
        int block_accessed; // number of blocks accessed

        int block_id; // id of block we are pointing to
        char* start_addr; // starting address

        reviewChain* deleted; // deleted records
};