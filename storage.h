#include <string>

#ifndef STORAGE_H
#define STORAGE_H

namespace Storage
{
    struct Record
    {
        char t_const[10];
        float averageRating;
        unsigned int numVotes;
    };
    struct DeletedChain
    {
        char* ptr;
        DeletedChain* next;
    };
    class MemoryPool
    {
    public:
        MemoryPool(unsigned int memory_size, unsigned int block_size);
        void insertRecord();
        void deleteRecord();

    private:
        // Pointer to the allocated space
        char* head;
        // For insertions/deletions, we will keep track of 
        // Pointer to current end (of space that has been filled)
        char* tail;
        // Chain of deleted records
        DeletedChain* deleted_chain;
    };
}

#endif