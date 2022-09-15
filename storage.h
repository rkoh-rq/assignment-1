// Commenting out this part because I don't want to overwrite in case this code still needed
//  #include <string>

// #ifndef STORAGE_H
// #define STORAGE_H

// namespace Storage
// {
//     struct Record
//     {
//         char t_const[10];
//         float averageRating;
//         unsigned int numVotes;
//     };
//     struct DeletedChain
//     {
//         char* ptr;
//         DeletedChain* next;
//     };
//     class MemoryPool
//     {
//     public:
//         MemoryPool(unsigned int memory_size, unsigned int block_size);
//         void insertRecord();
//         void deleteRecord();

//     private:
//         // Pointer to the allocated space
//         char* head;
//         // For insertions/deletions, we will keep track of 
//         // Pointer to current end (of space that has been filled)
//         char* tail;
//         // Chain of deleted records
//         DeletedChain* deleted_chain;
//     };
// }

// #endif

#include <iostream>
#include <fstream>
// using namespace std; 

struct reviewRecord{
    char t_const [10];
    float avg_rating;
    int num_votes; 
};

struct reviewAddress {
    int block_add; // block it is in
    short int offset; // from which part of memory in block it started 
};

struct deletedChain
{
    reviewAddress deletedAddr;
    deletedChain* next;
};

class Storage {
    public:
        Storage(std::size_t disk_size, std::size_t block_size); 
        
        // helper functions
        bool check_new_block(); // check if current block_size_used + block_size is more than disk_size
        reviewAddress record_get_block_add(); // returns block add to store to 
        // CRUD functions, without update 
        reviewRecord retrieve_record(reviewAddress ra); // retrieve record given reviewAddress and record size
        bool insert_record(reviewAddress ra, reviewRecord record); // store record
        bool remove_record(reviewAddress ra); // remove record, return false if error
        // get functions 
        int get_disk_size();
        int get_block_size(); 
        int get_total_blocks();
        int get_block_size_used();
        int get_internal_block_size_used(); 
        int get_actual_size_used();
        int get_block_id();

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

        deletedChain* deleted; // deleted records
};