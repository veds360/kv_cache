#include "KVSTORE.h"
#include<filesystem>
#include<fstream>
#include<regex>
#include<algorithm>
#include<cstdint>
using namespace std;

namespace fs=std::filesystem;


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
    sstables[obj.level].push_back(obj);
    memtable.clear();
}

bool KVStore::get(string key,string &value){
    Node* memres=memtable.get(key,value);
    if(memres){
        if(memres->tombstone) return false;

        value=memres->value;
        return true;
    }

    // for(int i=sstables.size()-1;i>=0;i--){
    //     SSTable curr=sstables[i];
    //     Node * res=curr.find(key);

    //     if(res){
    //         if(res->tombstone) return false;
    //         value=res->value;
    //         return true;
    //     }
    // }

    for(int i=0;i<NUM_LEVELS;i++){
        for(int j=sstables[i].size()-1;j>=0;j--){
            if(i==0){
                Node * curr=sstables[i][j].find(key);

                if(curr){
                    if(curr->tombstone) return false;
                    value=curr->value;
                    return true;
                }
            }
            else{
                string min_key=sstables[i][j].min_key;
                string max_key=sstables[i][j].max_key;

                if(min_key<=key && key<=max_key){
                    Node * curr=sstables[i][j].find(key);
                    
                    if(curr){
                        if(curr->tombstone) return false;
                        value=curr->value;
                        return true;
                    }
                }
                else continue;
            }
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


KVStore::KVStore(){
    regex pattern(R"(sstable_[0-9]+\.dat)");
    for(const auto&entry:fs::directory_iterator(".")){
        if(!entry.is_regular_file()) continue;
        string filename =entry.path().filename().string();

        if(regex_match(filename,pattern)){
            SSTable table;
            table.filename=filename;
            ifstream file(filename,ios::binary);
            int counter=0;
            uint8_t level;
            file.read(reinterpret_cast<char*>(&level),sizeof(level));
            table.level=level;
            while(file){
                uint32_t key_len;
                uint32_t value_len;
                uint8_t tombstone;
                streampos add;
                
                if(counter%10==0){
                    add=file.tellg();
                }

                if(!file.read(reinterpret_cast<char*>(&key_len),sizeof(key_len))) break;
                if(!file.read(reinterpret_cast<char*>(&value_len),sizeof(value_len))) break;
                string key;
                key.resize(key_len);
                file.read(&key[0],key_len);
                if(counter==0){
                    table.min_key=key;
                }
                table.max_key=key;
                string value;
                value.resize(value_len);
                file.read(&value[0],value_len);
                file.read(reinterpret_cast<char*>(&tombstone),sizeof(tombstone));

                if(counter%10==0){
                    table.index[key]=add;
                }
                counter++;

            }
            sstables[table.level].push_back(table);
        }
    }

    
    sort(sstables[0].begin(),sstables[0].end(),[](const SSTable&a,SSTable&b){
        int numA=stoi(a.filename.substr(8));
        int numB=stoi(b.filename.substr(8));
        return numA<numB;
    });
}
