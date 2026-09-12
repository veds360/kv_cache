#include "MemTable.h"
#include "SSTable.h"

void MemTable::put(std::string key, std::string value) {

    Node* node=tree.find(tree.root,key);
    if(node){
        curr_size-=node->value.size();
        curr_size+=value.size();
    }
    else{
        curr_size+=sizeof(Node);
        curr_size+=key.size();
        curr_size+=value.size();
    }
    tree.root = tree.insert(tree.root, key, value,0);

    if(curr_size>=MAX_SIZE){
        // string file_name="sstable_";
        // file_name+=std::to_string(sstable_count);
        // file_name += ".dat";
        // flush(file_name);

        should_flush=true;
    }
}

Node* MemTable::get(std::string key, std::string& value) {
    Node* node = tree.find(tree.root, key);

    return node;
}

void MemTable::del(std::string key) {
    // tree.root = tree.del(tree.root, key);
    Node * node=tree.find(tree.root,key);
    if(node){
        node->tombstone=1;
    }
    else{
        tree.root=tree.insert(tree.root,key,"",1);
        curr_size += sizeof(Node);
        curr_size += key.size();
    }

    if(curr_size>=MAX_SIZE){
        should_flush=true;
    }
}


void MemTable::clear(){
    tree.clear(tree.root);
    tree.root=NULL;
    curr_size=0;
    sstable_count++;
    should_flush=false;
}