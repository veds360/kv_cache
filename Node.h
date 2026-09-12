#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

struct Node {
    string key;
    string value;
    int height;

    Node* left;
    Node* right;

    bool tombstone;

    Node() : key(""), value(""), height(0),
             left(nullptr), right(nullptr),
             tombstone(false) {}
};

#endif