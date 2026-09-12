#include "KVSTORE.h"

using namespace std;


void KVStore::put(string key,string value){
    memtable.put(key,value);
    if(memtable.should_flush){
        this->flush();
    }
}

void KVStore::flush(){
    SSTable obj;
    vector<Node*>entries=memtable.tree.inorder(memtable.tree.root);
    string file_name="sstable_";
    file_name+=std::to_string(memtable.sstable_count);
    file_name += ".dat";
    obj.filename=file_name;
    obj.write_entries(entries);
    sstables.push_back(obj);
    memtable.clear();
}

bool KVStore::get(string key,string &value){
    Node* memres=memtable.get(key,value);
    if(memres){
        if(memres->tombstone) return false;

        value=memres->value;
        return true;
    }

    for(int i=sstables.size()-1;i>=0;i--){
        SSTable curr=sstables[i];
        Node * res=curr.find(key);

        if(res){
            if(res->tombstone) return false;
            value=res->value;
            return true;
        }
    }
    return false;
}

void KVStore::del(string key){
    memtable.del(key);

    if(memtable.should_flush){
        this->flush();
    }
}