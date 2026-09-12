#include "AVLTree.h"
#include <algorithm>
#include <cstdlib>
#include<iostream>
#include<bits/stdc++.h>
using namespace std;



int AVLTree::height(Node *nod){
    if(!nod) return -1;
    return nod->height;
}

int AVLTree::balanceFactor(Node * node){
    return height(node->left)-height(node->right);
}


bool cmp(string s1,string s2){
    return s1<s2;
}

Node* AVLTree::del(Node*root,string key){
    if(!root) return NULL;
    string curr=root->key;
    if(curr==key){
        root->tombstone=true;
        return root;
    }

    if(cmp(curr,key)){
        return del(root->right,key);
    }
    return del(root->left,key);
}

AVLTree::AVLTree() : root(nullptr) {}


Node * AVLTree::find(Node *root,string key){
    if(!root) return NULL;
    string curr=root->key;
    if(curr==key) return root;

    if(cmp(curr,key)) return find(root->right,key);

    return find(root->left,key);

}

Node *AVLTree::insert(Node* root,string key,string value,bool tombstone){
    if(!root){
        Node *croot=new Node;
        croot->height=0;
        croot->left=NULL;
        croot->right=NULL;
        croot->key=key;
        croot->value=value;
        croot->tombstone=tombstone;
        return croot;
    }

    string curr=root->key;

    if(curr==key){
        root->value=value;
        root->tombstone=tombstone;
        return root;
    }
    bool comparision=cmp(curr,key);
    if(comparision && root->right){
        Node *newrt=insert(root->right,key,value,tombstone);
        root->right=newrt;
        int rheight=root->right->height;
        int lheight=-1;
        if(root->left) lheight=root->left->height;

        int diff=abs(lheight-rheight);

        if(diff<=1){ 
            root->height=1+max(height(root->left),height(root->right));
            return root;
        }

        if(rheight>lheight){
            if(balanceFactor(root->right)<0){
                Node *newroot=root->right;
                root->right=newroot->left;
                newroot->left=root;
                
                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;

            }
            else{
                Node *newright=root->right->left;
                root->right->left=newright->right;
                newright->right=root->right;
                root->right=newright;
                newright->height=1+max(height(newright->left),height(newright->right));
                root->right->right->height=1+max(height(root->right->right->left),height(root->right->right->right));


                Node *newroot=root->right;
                root->right=newroot->left;
                newroot->left=root;
                
                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;

            }
        }
        else{
            if(balanceFactor(root->left)>0){
                Node*newroot=root->left;
                root->left=newroot->right;
                newroot->right=root;

                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;
            }
            else{
                Node* newleft=root->left->right;
                root->left->right=newleft->left;
                newleft->left=root->left;
                root->left=newleft;
                newleft->left->height=1+max(height(newleft->left->left),height(newleft->left->right));

                newleft->height=1+max(height(newleft->left),height(newleft->right));



                Node*newroot=root->left;
                root->left=newroot->right;
                newroot->right=root;

                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;

            }
        }

    }

    if(!comparision && root->left){
        Node*newleft=insert(root->left,key,value,tombstone);
        root->left=newleft;
        int lheight=root->left->height;
        int rheight=-1;
        if(root->right) rheight=root->right->height;

        int diff=abs(lheight-rheight);

        if(diff<=1){ 
            
            root->height=1+max(height(root->left),height(root->right));
            return root;

        }

        if(rheight>lheight){
            if(balanceFactor(root->right)<0){
                Node *newroot=root->right;
                root->right=newroot->left;
                newroot->left=root;

                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;
            }
            else{
                Node *newright=root->right->left;
                root->right->left=newright->right;
                newright->right=root->right;
                root->right=newright;
                newright->height=1+max(height(newright->left),height(newright->right));
                root->right->right->height=1+max(height(root->right->right->left),height(root->right->right->right));


                Node *newroot=root->right;
                root->right=newroot->left;
                newroot->left=root;
                
                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;
            }
        }
        else{
            if(balanceFactor(root->left)>0){
                Node*newroot=root->left;
                root->left=newroot->right;
                newroot->right=root;

                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;
            }
            else{
                 Node* newleft=root->left->right;
                root->left->right=newleft->left;
                newleft->left=root->left;
                root->left=newleft;
                newleft->left->height=1+max(height(newleft->left->left),height(newleft->left->right));

                newleft->height=1+max(height(newleft->left),height(newleft->right));



                Node*newroot=root->left;
                root->left=newroot->right;
                newroot->right=root;

                root->height=1+max(height(root->left),height(root->right));
                newroot->height=1+max(height(newroot->left),height(newroot->right));

                return newroot;
            }
        }
    }

    if(comparision && !root->right){
        Node* newnode=new Node;
        newnode->left=NULL;
        newnode->right=NULL;
        newnode->key=key;
        newnode->value=value;
        newnode->height=0;
        newnode->tombstone=tombstone;
        root->right=newnode;
        int lheight=0;
        if(root->left) lheight=root->left->height+1;
        root->height=max(lheight,1);

        return root;
    }

    if(!comparision && !root->left){
        Node *newnode=new Node;
        newnode->tombstone=tombstone;
        newnode->left=NULL;
        newnode->right=NULL;
        newnode->key=key;
        newnode->value=value;
        newnode->height=0;
        root->left=newnode;
        int rheight=0;
        if(root->right) rheight=root->right->height+1;

        root->height=max(rheight,1);

        return root;
    }
}



vector<Node*> AVLTree::inorder(Node * root){
    vector<Node*>trav;
    inorderHelper(root,trav);
    return trav;
}

void AVLTree::inorderHelper(Node *root,vector<Node*>&traversal){
    if(!root) return;

    if(root->left) inorderHelper(root->left,traversal);

    traversal.push_back(root);

    if(root->right) inorderHelper(root->right,traversal);
}


void AVLTree::clear(Node*root){
    if(!root) return;
    clear(root->left);
    clear(root->right);
    delete root;
}