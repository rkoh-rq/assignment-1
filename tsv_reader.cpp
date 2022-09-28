// tsv_reader.cpp
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "tsv_reader.h"

TSVReader::TSVReader(std::string fileName)
{
    assert(fileName.compare(fileName.length() - 5, 5, ".tsv"));
    dataFile.open(fileName);
    // Skip the header
    getNextRow();
}

bool TSVReader::end()
{
    return dataFile.eof();
}

std::vector<std::string> TSVReader::getNextRow()
{
    if (!end()){
        std::string str;
        std::getline( dataFile, str);
        std::stringstream buffer(str);
        std::vector<std::string> temp(3);
        std::getline( buffer, temp[0], '\t');
        std::getline( buffer, temp[1], '\t');
        std::getline( buffer, temp[2], '\t');
        return temp;
    }
}

void TSVReader::printSomeData(int rows_to_print)
{
    // For testing purposes only
    // This loop prints an extra row on top of rows_to_print,
    // which is the column labels.
    std::vector<std::string> row;
    for (int i= 0; i < rows_to_print + 1; i++){
        row = getNextRow();
        for (int j = 0; j < 3; j++) {
            std::cout << row[j] << "\t";
        }
        std::cout << "\n";
    }
}
