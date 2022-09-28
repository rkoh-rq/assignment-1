#include "tsv_reader.h"
#include "storage.h"
#include "bPlusTree.h"

#include <cstring>
#include <math.h>

int main() {
    // Get block size
    int block_size = 0;
    std::cout << "----------------------------------------" << std::endl; 
    std::cout << "Choose the size for a block" <<std::endl; 

    int block_choice = 0;
    
    while(block_choice != 1 && block_choice != 2){
        std::cout << "Select a size: "<<std::endl; 
        std::cout << "1: 200B" <<std::endl;
        std::cout << "2: 500B" <<std::endl;
        std::cin >> block_choice; 

        if (int(block_choice) == 1){
            block_size = int(200);
        }
        else if(int(block_choice) == 2){
            block_size = int(500);
        }
        else{
            std::cin.clear();
            std::cout << "Please select only 1 or 2" <<std::endl; 
        }
    }
    // Clear buffer(?) 
    // streambuf *coutbuf = std::cout.rdbuf(); 

    // Create storage space for both disk and index
    // Total 500MB: 150MB used for records storage
    // Initialise records storage
    int disk_size = 100000000;
    Storage records_storage = Storage(disk_size, block_size);
    
    // Initialise B+ tree storage
    BPTree bplustree = BPTree();

    /* 
        -----------------------------------------------------------------------------
        Experiment 1
        Store data and report:
            - No. of blocks
            - Size of database (in MB)
        -----------------------------------------------------------------------------
    */


    std::cout << "------------ Experiment 1 ------------ " << std::endl; 
    std::cout << "Insert records and count number of blocks and size..." << std::endl; 
    std::cout << "Resetting acessed blocks..." << std::endl; 
    // Reset blocks accessed 
    records_storage.reset_blocks(); 

    std::cout << "Initialising TSV Reader..." << std::endl; 
    // Initialise TSV Reader
    std::string data_file = "data.tsv";
    TSVReader reader = TSVReader(data_file);

    std::cout << "Inserting records..." << std::endl; 
    // Check if last line 
    bool check_end = true;
    while(check_end){
        std::vector<std::string> temp = reader.getNextRow();
        if (temp[0] == ""){
            std::cout << "Stop Inserting Records..." << std::endl; 
            check_end = false; 
            break;
        }
        reviewRecord record;
        reviewAddress addr; 
        // Get data from tsv to store in records
        std::strcpy(record.t_const, temp[0].c_str());
        record.avg_rating = std::stof(temp[1]);
        record.num_votes = std::stoi(temp[2]);
        std::size_t record_size = sizeof(record);

        // Get record address
        addr = records_storage.record_get_block_add(record_size); 
        // Insert record 
        records_storage.insert_record(addr, record, record_size);
        // std::cout << "Insert at " << addr.block_add << " | " << addr.offset << "\t|\t" << record.t_const << "\t" << record.avg_rating << "\t" << record.num_votes <<"\n";

    }

    // print statistics
    std::cout << "Statistics of number of blocks and size of database..." << std::endl; 
    int total_blocks = records_storage.get_total_blocks();
    std::cout << "Total number of blocks: " << total_blocks << std::endl; 

    int blocks_used = records_storage.get_allocated_blocks();
    int db_size_used = blocks_used * block_size; 
    std::cout << "Size of database in B: " << db_size_used << std::endl;
    
    std::cout << "Total number of blocks used: " << blocks_used << std::endl;
    float total_db_size = db_size_used / pow(10, 6);
    std::cout << "Size of database in MB: " << total_db_size << std::endl;

    /* 
        -----------------------------------------------------------------------------
        Experiment 2
        Build B+ Tree on numVotes and insert records sequentially and report:
            - Parameter n of B+ Tree
            - Number of nodes of the B+ Tree
            - Height of B+ Tree
            - Content of root node and 1st child node
        -----------------------------------------------------------------------------
    */

    std::cout << "------------ Experiment 2 ------------ " << std::endl; 
    std::cout << "B+ Tree on numVotes and insert records sequentially..." << std::endl; 

    return 0;
}