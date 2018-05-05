#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

class FileParser{
public:
    ifstream file;
    string fileName;
    bool endOfFile = false;
    
public:
    FileParser(string fileName);
    ~FileParser();

    bool getNext(uint64_t &pc, uint64_t &addr);
    bool eof();
};

#endif /* FILEPARSER_H */
