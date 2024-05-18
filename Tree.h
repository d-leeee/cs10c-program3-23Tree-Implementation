#pragma once

#include "Node.h"

class Tree {
    private:
        Node *root;
        bool empty() const { return !root; }    // Return if empty tree

    public:
        Tree() : root(nullptr) {}   // Default constructor

        // Rule of three
        ~Tree() { destroyTree(root); }
        void destroyTree(Node* node){
            if (!node){
                return;
            }

            destroyTree(node->left);    // Destroy left subtree
            destroyTree(node->middle);  // Destroy middle subtree
            destroyTree(node->right);   // Destroy right subtree

            // Delete current node
            delete node;
        }
        Tree(const Tree&) = delete; 
        Tree& operator=(const Tree&) = delete;

        // Mutator functions
        void insert(const string& key);   // Insert a new key into the tree
        void remove(const string& key);   // Remove a key from the tree

        // Accessor functions
        bool search(const string& key) const;         // Search for the existance of a key in the tree
        void preOrder() const { preOrder(root); }     // Print tree in preorder notation
        void inOrder() const { inOrder(root); }       // Print tree in inorder notation
        void postOrder() const { postOrder(root); }   // Print tree in postorder notation

    private:
        // Helper functions
        void split(Node* node, const string& keyToMoveUp);        // Split a node into 2 and move a node to the parent (used by insert function)
        void merge(Node* node);                                   // Merge 2 nodes into 1 by bringing a parent down (used by remove function)
        void reorderTwoKeys(Node* node);                          // Reorder a node's keys from smallest to largest (used when changing a node's keys)
        string reorderThreeKeys(Node* node, const string& key);   // When a node is full and needs to insert, reorder the node and return the key to move up to parent (used by insert function)
        Node* prepareOperation(const string& key) const;          // Search for the node to access and mutate (used by search, insert, remove functions)

        // Print functions
        void preOrder(Node* node) const;    // Recursively print preorder
        void inOrder(Node* node) const;     // Recursively print inorder
        void postOrder(Node* node) const;   // Recursively print postorder
};