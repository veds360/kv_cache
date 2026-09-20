#ifndef COMPACTOR_H
#define COMPACTOR_H

#include <vector>
#include "SSTable.h"

using namespace std;

class Compactor {
    vector<vector<SSTable>>& sstables;

public:
    Compactor(vector<vector<SSTable>>& sstables);
};

#endif