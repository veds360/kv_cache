#ifndef MEMTABLE_H
#define MEMTABLE_H

#include "AVLTree.h"
#include <string>

class MemTable {
public:
    AVLTree tree;
    size_t curr_size=0;
    int sstable_count=0;
    bool should_flush=false;
    static const size_t MAX_SIZE = 1 * 1024 * 1024; // 1 MiB currently


    void put(std::string key, std::string value);
    Node*get(std::string key, std::string& value);
    void del(std::string key);
    void clear();
};

#endif


// look at string represenation in cpp along with sizeof string