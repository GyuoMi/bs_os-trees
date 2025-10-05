#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <cmath>
#include <ostream>
#include <fstream>


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

int treeHeight(Node *node){
    if (node == nullptr){
        return -1;
    }
    int leftHeight = treeHeight(node->left);
    int rightHeight = treeHeight(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}

void freeTree(Node *node){
    if (node == nullptr){
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}
//  end of subroutines/helpers

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

int main(){
    srand(time(0));

    std::ofstream outFile;
    outFile.open("bst_results.csv");

    outFile << "n,ExpectedHeight,AvgBuildTime_ms,AvgDestroyTime_ms,AvgWalkTime_ms\n";

    // int n_vals[] = {100, 500, 1000, 2000, 5000, 10000, 50000, 100000} ;
    int n_vals[] = {100000, 500000, 1000000} ;

    int n_trials = 50;

    for (int i = 0; i < sizeof(n_vals) / sizeof(n_vals[0]); ++i){
        int n = n_vals[i];

        double total_height = 0;
        double total_build_time = 0;
        double total_destroy_time = 0;
        double total_walk_time = 0;

        for (int j = 0; j < n_trials; ++j){
            Node *root = nullptr;

            int *keys = new int[n];
            // just filling the array from 0 to n-1
            for (int i = 0; i < n; ++i){
                keys[i] = i;
            }

            // rand swap/shuffle
            for (int k = n - 1; k > 0; --k){
                int swap_idx = rand() % (k + 1);

                int temp = keys[k];
                keys[k] = keys[swap_idx];
                keys[swap_idx] = temp;
            }

            // (iii) - (v)
            clock_t start_build = clock();
            for (int k = 0; k < n; ++k){
                treeInsert(root, new Node(keys[k]));
            }

            clock_t end_build = clock();
            total_build_time += (double)(end_build - start_build) * 1000.0 / CLOCKS_PER_SEC;

            total_height += treeHeight(root);

            clock_t start_walk = clock();
            inOrderTreeWalk(root);
            clock_t end_walk = clock();
            total_walk_time += (double)(end_walk - start_walk) * 1000.0 / CLOCKS_PER_SEC;

            clock_t start_destroy = clock();
            while (root != nullptr){
                Node *node_to_delete = root;
                treeDelete(root, node_to_delete);
                delete node_to_delete;
            }
            clock_t end_destroy = clock();
            total_destroy_time += (double)(end_destroy - start_destroy) * 1000.0 / CLOCKS_PER_SEC;

            delete[] keys;
        }

        double expected_height = total_height / n_trials;
        double avg_build_time = total_build_time / n_trials;
        double avg_destroy_time = total_destroy_time / n_trials;
        double avg_walk_time = total_walk_time / n_trials;

        outFile << n << "," << expected_height << "," << avg_build_time << "," << avg_destroy_time 
        << "," << avg_walk_time << std::endl;
    }
    outFile.close();
    return 0;
}
