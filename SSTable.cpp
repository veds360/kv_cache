#include "SSTable.h"
#include<fstream>
#include<cstdint>
using namespace std;

void SSTable::write_entries(vector<Node*>entries){
    ofstream file(filename,ios::binary);
    int counter=0;
    uint8_t level=this->level;
    file.write(reinterpret_cast<char*>(&level),sizeof(level)); 
    for(Node * node:entries){
        uint32_t key_length=node->key.size();
        uint32_t val_length=node->value.size();
        uint8_t tomb=node->tombstone;

        if(counter%10==0){
            streampos offset=file.tellp();
            index[node->key]=offset;
        }

        if(counter==0){
            min_key=node->key;
        }

        if(counter==entries.size()-1){
            max_key=node->key;
        }

        file.write(reinterpret_cast<char *>(&key_length),sizeof(key_length));

        file.write(reinterpret_cast<char*>(&val_length),sizeof(val_length));

        file.write(node->key.data(),key_length);
        file.write(node->value.data(),val_length);
        file.write(reinterpret_cast<char*>(&tomb),sizeof(tomb));
        counter++;
    }
    file.close();
}


vector<Node*> SSTable::read_entries(){

    vector<Node*>ret;
    ifstream file(filename,ios::binary);
    uint32_t key_len;
    
    while(file.read(reinterpret_cast<char *>(&key_len),4)){
        uint32_t val_len;
        uint8_t tombstone;

        file.read(reinterpret_cast<char*>(&val_len),4);
        string key;
        key.resize(key_len);
        file.read(&key[0],key_len);
        string value;
        value.resize(val_len);
        file.read(&value[0],val_len);

        file.read(reinterpret_cast<char*>(&tombstone),1);

        Node * newnode= new Node;
        newnode->key=key;
        newnode->value=value;
        newnode->tombstone=tombstone;

        ret.push_back(newnode);
    }
    return ret;

}

Node* SSTable::find(string key) {
    ifstream file(filename, ios::binary);

    auto it = index.upper_bound(key);

    if (it != index.begin())
        --it;

    file.seekg(it->second);

    while (file) {
        uint32_t key_len;
        uint32_t val_len;
        uint8_t tombstone;

        if (!file.read(reinterpret_cast<char*>(&key_len), sizeof(key_len)))
            break;

        if (!file.read(reinterpret_cast<char*>(&val_len), sizeof(val_len)))
            break;

        string current_key(key_len, '\0');
        file.read(&current_key[0], key_len);

        string value(val_len, '\0');
        file.read(&value[0], val_len);

        file.read(reinterpret_cast<char*>(&tombstone), sizeof(tombstone));

        if (current_key == key) {
            Node* node = new Node;
            node->key = current_key;
            node->value = value;
            node->tombstone = tombstone;
            return node;
        }

        // Since SSTable is sorted, we've gone past the key
        if (current_key > key)
            break;
    }

    return nullptr;
}

