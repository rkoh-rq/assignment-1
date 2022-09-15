#include "tsv_reader.h"
#include "storage.h"
#include <string>

int main() {
    std::string data_file = "data.tsv";
    TSVReader reader = TSVReader(data_file);

    // // Test tsv reader
    reader.printSomeData(3);

    // TSV reader to storage
    Storage storage = Storage(100000000, 200);
    reviewRecord record;
    reviewAddress addr;
    for (int i = 0; i < 3; i++){
        std::vector<std::string> temp = reader.getNextRow();
        std::strcpy(record.t_const, temp[0].c_str());
        record.avg_rating = std::stof(temp[1]);
        record.num_votes = std::stoi(temp[2]);
        addr = storage.record_get_block_add();
        storage.insert_record(addr, record);
    }
    reviewRecord check = storage.retrieve_record(addr);
    std::cout << "Retrieved record:\t" << check.t_const << "\t" << check.avg_rating << "\t" << check.num_votes << "\t";
}