#pragma once

#include "Node.h"

class Tree {
    private:
        Node *root;
        bool empty() const { return !root; }

    public:
        Tree() : root(nullptr) {}
        //~Tree();
        Tree(const Tree&) = delete;
        Tree& operator=(const Tree&) = delete;

        void insert(const string& value);
        void remove(const string& value);
        bool search(const string& value) const;
        void preOrder() const { preOrder(root); }
        void inOrder() const { inOrder(root); }
        void postOrder() const { postOrder(root); }

    private:
        void split(Node* node, const string& keyToMoveUp);
        void merge(Node* node);
        void reorderTwoKeys(Node* node);
        string reorderThreeKeys(Node* node, const string& value);
        Node* prepareOperation(const string& value) const;

        //order functions
        void preOrder(Node* node) const;
        void inOrder(Node* node) const;
        void postOrder(Node* node) const;

};