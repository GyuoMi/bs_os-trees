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
    int size;

    Node(int k)
    {
        key = k;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        size = 1;
    }
};

void treeInsert(Node *&root, Node *z)
{
    Node *y = nullptr;
    Node *x = root;

    while (x != nullptr)
    {
        y = x;
        // increment size of nodes as we traverse path
        y->size++;
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

// decrement ancestors for treeDelete
void decrementAncestors(Node *node){
    while (node != nullptr){
        node->size--;
        node = node->parent;
    }
}
//  end of subroutines/helpers

void treeDelete(Node *&root, Node *z)
{
    if (z == nullptr) return;

    Node *node_to_update = nullptr;

    if (z->left == nullptr){
        // case where z has no left child
        node_to_update = z->parent;
        transplant(root, z, z->right);
    }
    else if (z->right == nullptr){
        // case where z has a left child but no right child
        node_to_update = z->parent;
        transplant(root, z, z->left);
    }
    else {
        // case where z has two children
        Node* y = treeMinimum(z->right);
        // y is not z's direct child, y will be removed
        if (y->parent != z){
            node_to_update = y->parent;
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        else{
            // if y is z's direct child, y should replace z
            node_to_update = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;

        // update y's size based on it's new children
        int left_size = 0;
        if (y->left != nullptr) {
            left_size = y->left->size;
        }
        
        int right_size = 0;
        if (y->right != nullptr) {
            right_size = y->right->size;
        }
        
        y->size = left_size + right_size + 1;    
    }
    decrementAncestors(node_to_update);
}

void inOrderTreeWalk(Node *x){
    if (x != nullptr){
        inOrderTreeWalk(x->left);
        std::cout << x->key << " ";
        inOrderTreeWalk(x->right);
    }
}

Node* OS_SELECT(Node *x, int i){
    if (x == nullptr || i <= 0){
        return nullptr;
    }
    
    // so we do this check here since the normal RB tree pseudocode does the following
    // it checks the left child of x's size without checking if it is a nil i.e. nullptr
    // accessing mem that doesnt exist thus segfaulting
    int left_size;
    if (x->left != nullptr){
        left_size = x->left->size;
    }
    else {
        left_size = 0;
    }
    int r = left_size + 1;

    if (i == r){
        return x;
    }
    else if (i < r){
        return OS_SELECT(x->left, i);
    }
    else return OS_SELECT(x->right, i - r);
}

int OS_RANK(Node *root, Node *x){
    if (x == nullptr){
        return 0;
    }

    int left_size;
    if (x->left != nullptr){
        left_size = x->left->size;
    }
    else {
        left_size = 0;
    }
    int r = left_size + 1;

    Node *y = x;
    while (y != root){
        if (y == y->parent->right){
            r = r + y->parent->left->size + 1;
        }
        y = y->parent;
    }
    return r;
}

int main(){
    srand(time(0));

    std::ofstream outFile;
    outFile.open("ost_results.csv");

    outFile << "n,AvgBuildTime_ms,AvgDestroyTime_ms,AvgSelectTime_ms,AvgRankTime_ms\n";
    // int n_vals[] = {100, 500, 1000, 2000, 5000, 10000, 50000, 100000} ;
    int n_vals[] = {100000, 500000, 1000000} ;
    int n_trials = 50;

    for (int i = 0; i < sizeof(n_vals) / sizeof(n_vals[0]); ++i){
        int n = n_vals[i];

        double total_height = 0;
        double total_build_time = 0;
        double total_destroy_time = 0;
        double total_walk_time = 0;
        double total_select_time = 0;
        double total_rank_time = 0;

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

            // clock_t start_walk = clock();
            // inOrderTreeWalk(root);
            // clock_t end_walk = clock();
            // total_walk_time += (double)(end_walk - start_walk) * 1000.0 / CLOCKS_PER_SEC;

            clock_t start_select = clock();
            OS_SELECT(root, n / 2);
            clock_t end_select = clock();
            total_select_time += (double)(end_select - start_select) * 1000.0 / CLOCKS_PER_SEC;

            clock_t start_rank = clock();
            OS_RANK(root, root); 
            clock_t end_rank = clock();
            total_rank_time += (double)(end_rank - start_rank) * 1000.0 / CLOCKS_PER_SEC;


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

        double avg_build_time = total_build_time / n_trials;
        double avg_destroy_time = total_destroy_time / n_trials;
        double avg_select_time = total_select_time / n_trials;
        double avg_rank_time = total_rank_time / n_trials;

        outFile << n <<  "," << avg_build_time << "," << avg_destroy_time 
        << "," << avg_select_time << "," << avg_rank_time << std::endl;
    }
    outFile.close();
    return 0;
}
