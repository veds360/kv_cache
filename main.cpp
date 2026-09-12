#include "MemTable.h"
#include <iostream>
#include "SSTable.h"
#include <vector>
#include "KVSTORE.h"
using namespace std;

int main() {
    KVStore store;
    string value,val1;
   for (int i = 0; i < 50; i++) {
    store.put("key" + to_string(i), "value" + to_string(i));
}

store.flush();

if (store.get("key37", value))
    cout << value << endl;
else
    cout << "not found" << endl;
}