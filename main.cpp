#include "tsv_reader.h"
#include "storage.h"
#include "bPlusTree.cpp"

#include <cstring>
#include <math.h>

int main() {
    // Get block size
    int block_size = 0;
    std::cout << "----------------------------------------" <<endl; 
    std::cout << "Choose the size for a block" <<endl; 

    int block_choice = 0;
    
    while(block_choice != 1 && block_choice != 2){
        std::cout << "Select a size: "<<endl; 
        std::cout << "1: 200B" <<endl;
        std::cout << "2: 500B" <<endl;
        cin >> block_choice; 

        if (int(block_choice) == 1){
            block_size = int(200);
        }
        else if(int(block_choice) == 2){
            block_size = int(500);
        }
        else{
            cin.clear();
            std::cout << "Please select only 1 or 2" <<endl; 
        }
    }
    // Clear buffer(?) 
    // streambuf *coutbuf = std::cout.rdbuf(); 

    // Create storage space for both disk and index
    // Total 500MB: 150MB used for records storage, 350MB used for index storage
    // Initialise records storage
    int records_size = 150000000;
    Storage records_storage = Storage(records_size, block_size);
    
    // Initialise B+ tree storage
    int index_size = 350000000;
    Storage index_storage = Storage(index_size, block_size);
    BPTree bplustree = BPTree();

    /* 
        -----------------------------------------------------------------------------
        Experiment 1
        Store data and report:
            - No. of blocks
            - Size of database (in MB)
        -----------------------------------------------------------------------------
    */


    std::cout << "------------ Experiment 1 ------------ " << endl; 
    std::cout << "Insert records and count number of blocks and size..." << endl; 
    std::cout << "Resetting acessed blocks..." << endl; 
    // Reset blocks accessed 
    records_storage.reset_blocks();
    index_storage.reset_blocks();  

    std::cout << "Initialising TSV Reader..." << endl; 
    // Initialise TSV Reader
    std::string data_file = "data.tsv";
    TSVReader reader = TSVReader(data_file);

    std::cout << "Inserting records..." << endl; 
    // Check if last line 
    bool check_end = true;
    while(check_end){
        std::vector<std::string> temp = reader.getNextRow();
        if (temp[0] == ""){
            std::cout << "Stop Inserting Records..." << endl; 
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
    std::cout << "Statistics of number of blocks and size of database..." << endl; 
    int total_blocks = records_storage.get_total_blocks();
    std::cout << "Total number of blocks: " << total_blocks << endl; 
    int db_size_used = records_storage.get_actual_size_used(); 
    std::cout << "Size of database in B: " << db_size_used << endl;


    int blocks_used = records_storage.get_allocated_blocks();
    std::cout << "Total number of blocks used: " << blocks_used << endl;
    float total_db_size = db_size_used / pow(10, 6);
    std::cout << "Size of database in MB: " << total_db_size << endl;

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

    std::cout << "------------ Experiment 2 ------------ " << endl; 
    std::cout << "B+ Tree on numVotes and insert records sequentially..." << endl; 


}