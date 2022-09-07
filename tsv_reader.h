// tsv_reader.h
#ifndef TSVREADER_H
#define TSVREADER_H
#include <fstream>
#include <vector>
#include <string>

class TSVReader
{
    std::string _fileName;
    std::ifstream dataFile;
public:
    TSVReader(std::string fileName);
    void printSomeData(int rows_to_print);
    std::vector<std::string> getNextRow();
};

#endif