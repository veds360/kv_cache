#ifndef AVLTREE_H
#define AVLTREE_H

#include "Node.h"
#include <string>
#include <vector>
using namespace std;
class AVLTree {
public:
    Node* root;

    AVLTree();

    Node* insert(Node* root, std::string key, std::string value,bool tombstone);
    Node* find(Node* root, std::string key);
    Node* del(Node* root, std::string key);

    int height(Node* nod);
    int balanceFactor(Node* node);

    vector<Node*> inorder(Node * root);
    void inorderHelper(Node *root,vector<Node*>&traversal);
    void clear(Node*root);
};

#endif