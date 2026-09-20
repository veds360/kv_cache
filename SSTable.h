#ifndef SSTABLE_H
#define SSTABLE_H

#include "AVLTree.h"
#include "Node.h"
#include<string>
#include<vector>
#include<map>

using namespace std;
class SSTable{
   
    public:
        int level=0;
        string min_key;
        string max_key;
        std::string filename;
        map<string,streampos> index;
        void write_entries(std::vector<Node*>entries);
        Node * find(string key);
        vector<Node*> read_entries();
};

#endif