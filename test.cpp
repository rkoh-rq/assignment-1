#include "tsv_reader.h"
#include <string>

int main() {
    std::string data_file = "data.tsv";
    TSVReader reader = TSVReader(data_file);
    reader.printSomeData(10);
}