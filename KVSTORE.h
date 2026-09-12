#ifndef KVSTORE_H
#define KVSTORE_H
#include "MemTable.h"
#include "SSTable.h"
class KVStore {
private:
    MemTable memtable;
    std::vector<SSTable> sstables;

public:
    void put(std::string key, std::string value);
    bool get(std::string key, std::string& value);
    void del(std::string key);
    void flush();
};

#endif