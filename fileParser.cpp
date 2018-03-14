#include "fileParser.h"

FileParser::FileParser(string fileName){
    this->fileName = fileName;

    file.open(fileName);
}


FileParser::~FileParser(){
    file.close();
}

bool FileParser::getNext(uint64_t &pc, uint64_t &addr){
    char buffer[256];
    char pc_s[20];
    char addr_s[20];

    file.getline(buffer, sizeof(buffer));
    int ret = sscanf(buffer,"%s %*1s %s",pc_s, addr_s);
    if(ret < 2){
        endOfFile = true;
        return false;
    }

    pc = stoul(pc_s, nullptr, 16);
    addr = stoul(addr_s, nullptr, 16);
}

bool FileParser::eof(){
    endOfFile || !file.good();
}