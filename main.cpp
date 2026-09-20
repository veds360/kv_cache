#include "MemTable.h"
#include <iostream>
#include "SSTable.h"
#include <vector>
#include "KVSTORE.h"
using namespace std;

int main() {
    KVStore store;
    string value,val1;
    store.get("bread",value);
    cout<<value<<endl;

}