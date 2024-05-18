#pragma once

#include <string>

using std::string;

class Node {

    friend class Tree;

    public:
        Node(const string& key) : small(key), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr){}

    private:
        string small;
        string large;

        Node *left;
        Node *middle;
        Node *right;
        Node *parent;
};