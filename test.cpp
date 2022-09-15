#include "tsv_reader.h"
#include "storage.h"
#include <string>

int main() {
    std::string data_file = "data.tsv";
    TSVReader reader = TSVReader(data_file);

    // TSV reader to storage
    Storage storage = Storage(100000000, 200);
    reviewRecord record;
    reviewAddress addr[20];
    for (int i = 0; i < 12; i++){
        std::vector<std::string> temp = reader.getNextRow();
        std::strcpy(record.t_const, temp[0].c_str());
        record.avg_rating = std::stof(temp[1]);
        record.num_votes = std::stoi(temp[2]);
        addr[i] = storage.record_get_block_add();
        storage.insert_record(addr[i], record);
        std::cout << "Insert at " << addr[i].block_add << " | " << addr[i].offset << "\t|\t" << record.t_const << "\t" << record.avg_rating << "\t" << record.num_votes <<"\n";
    }
    reviewRecord check = storage.retrieve_record(addr[0]);
    std::cout << "Retrieved first record:\t" << check.t_const << "\t" << check.avg_rating << "\t" << check.num_votes << "\n";

    printf("Removing items 2 to 5\n");
    storage.remove_record(addr[1]);
    storage.remove_record(addr[2]);
    storage.remove_record(addr[3]);
    storage.remove_record(addr[4]);

    for (int i = 12; i < 20; i++){
        std::vector<std::string> temp = reader.getNextRow();
        std::strcpy(record.t_const, temp[0].c_str());
        record.avg_rating = std::stof(temp[1]);
        record.num_votes = std::stoi(temp[2]);
        addr[i] = storage.record_get_block_add();
        storage.insert_record(addr[i], record);
        std::cout << "Insert at " << addr[i].block_add << " | " << addr[i].offset << "\t|\t" << record.t_const << "\t" << record.avg_rating << "\t" << record.num_votes <<"\n";
    }
}