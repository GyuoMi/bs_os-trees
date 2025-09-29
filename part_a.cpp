#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <cmath>

struct Node;

struct Node
{
    int key;
    Node *parent;
    Node *left;
    Node *right;

    Node(int k)
    {
        key = k;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }
};

void treeInsert(Node *&root, Node *z)
{
    Node *y = nullptr;
    Node *x = root;

    while (x != nullptr)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == nullptr)
    {
        root = z;
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else{
        y->right = z;
    }
}

// subroutines functions for delete
void transplant(Node *&root, Node *u, Node *v){
    if (u->parent == nullptr){
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

Node* treeMinimum(Node *x){
    while (x->left != nullptr){
        x = x->left;
    }
    return x;
}
// --- end of subroutines -- 

void treeDelete(Node *&root, Node *z)
{
    if (z->left == nullptr){
        transplant(root, z, z->right);
    }
    else if (z->right == nullptr){
        transplant(root, z, z->left);
    }
    else {
        Node* y = treeMinimum(z->right);
        if (y->parent != z){
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
}

void inOrderTreeWalk(Node *x){
    if (x != nullptr){
        inOrderTreeWalk(x->left);
        std::cout << x->key << " ";
        inOrderTreeWalk(x->right);
    }
}
