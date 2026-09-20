#ifndef KVSTORE_H
#define KVSTORE_H
#include "MemTable.h"
#include "SSTable.h"
#define NUM_LEVELS 3

using namespace std;
class KVStore {
private:
    MemTable memtable;
    //std::vector<SSTable> sstables;
    std::vector<std::vector<SSTable>>sstables{NUM_LEVELS};

public:
    KVStore();
    void put(std::string key, std::string value);
    bool get(std::string key, std::string& value);
    void del(std::string key);
    void flush();
};

#endif