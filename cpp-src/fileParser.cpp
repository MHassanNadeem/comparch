#include "fileParser.h"

FileParser::FileParser(string fileName){
    this->fileName = fileName;

    file.open(fileName, std::fstream::in);

    if(!file || file.bad()){
        throw std::runtime_error("ERROR: Could not open file");
    }
}


FileParser::~FileParser(){
    file.close();
}

bool FileParser::getNext(uint64_t &pc, uint64_t &addr){
    char buffer[100];
    char pc_s[20];
    char addr_s[20];

    if( !file.getline(buffer, sizeof(buffer)) )  goto error;
    if( sscanf(buffer,"%s %*1s %s",pc_s, addr_s) < 2 ) goto error;

    pc = stoul(pc_s, nullptr, 16);
    addr = stoul(addr_s, nullptr, 16);

    return true;

    error:
        endOfFile = true;
        return false;
}

bool FileParser::eof(){
    endOfFile || !file.good();
}