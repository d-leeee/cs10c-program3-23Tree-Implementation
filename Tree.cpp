#include "Tree.h"
#include <iostream>

using namespace std;

void Tree::insert(const string& value){
    //empty tree
    if (empty()){
        root = new Node(value);
        return;
    }

    Node* insertPlace = prepareOperation(value);

    //if large key is empty
    if (insertPlace->large.empty()){
        insertPlace->large = value;
        reorderTwoKeys(insertPlace);
    }
    else {
        //if large key is full
        string keyToMoveUp = reorderThreeKeys(insertPlace, value); //order small, large, and new keys
        split(insertPlace, keyToMoveUp); //move keyToMoveUp to parent node and split the tree
    }
}

void Tree::remove(const string& value){
    //return error if value DNE
    if (!search(value)){
        throw runtime_error("Value does not exist.");
    }

    Node* victim = prepareOperation(value);

    //CASE 1: if value is in leaf node
    if (!victim->left && !victim->middle && !victim->right){
        //if the value is equal to small key
        if (victim->small == value) {
            //if value is single value node
            if (victim->large.empty()){ 
                //if node is not root, update parent pointers
                if (victim == root){
                    delete root;
                    root = nullptr;
                }
                else {
                    merge(victim);
                }
            }
            //if value is two value node
            else { 
                victim->small.clear();
                reorderTwoKeys(victim);
            }
        }
        //if the value is equal to large key
        else { 
            victim->large.clear();
        }
    }

    //CASE 2: if value is not a leaf node
    else {
        //look for a successor
        Node* successor; 
        if (victim->small == value){
            successor = victim->middle;
        }
        else {
            successor = victim->right;
        }
        while (successor->left){
            successor = successor->left;
        }
        //replace value by successor value
        if (victim->small == value){
            victim->small = successor->small;
        }
        else {
            victim->large = successor->small;
        }
        reorderTwoKeys(victim);

        //if successor only has one value, merge
        if (successor->large.empty()){
            merge(successor);
        }
        //if successor has two values, remove small key
        else {
            successor->small.clear();
            reorderTwoKeys(successor);
        }    
    }
}

bool Tree::search(const string& value) const {
    throw runtime_error("Haven't implemented search function");
    return false;
}


void Tree::split(Node* node, const string& keyToMoveUp){
    Node* leftNode = new Node(node->small);
    Node* rightNode = new Node(node->large);

    //if node is root
    if (node == root){
        root = new Node(keyToMoveUp);
        root->left = leftNode;
        root->middle = rightNode;
        leftNode->parent = root;
        rightNode->parent = root;
    }
    //if node's parent is not full
    else if (node->parent->large.empty()){
        node->parent->large = keyToMoveUp;
        node->parent->middle = leftNode;
        node->parent->right = rightNode;
        reorderTwoKeys(node->parent);
    }

    delete node;
}

void Tree::merge(Node* node){
    Node* parent = node->parent;

    //if node is a left child
    if (parent->left == node){
        //if sibling is full, move parent small to node, siblings large to parent
        if (!parent->middle->large.empty()){
            node->small = parent->small;
            parent->small = parent->middle->small;
            parent->middle->small.clear();
            reorderTwoKeys(parent->middle);
        }
        //if sibling is not full, merge node with sibling
        else {
            node->small = parent->small;
            node->large = parent->middle->small;
            parent->small.clear();
            parent->middle = nullptr;
            reorderTwoKeys(node);
            delete parent->middle;
        }
    }

    //if node is a middle child
    else if (parent->middle == node){
        //if sibling is full, move parent small to node, siblings large to parent
        if (!parent->left->large.empty()){
            node->small = parent->small;
            parent->small = parent->left->large;
            parent->left->large.clear();
        }
        //if sibling is not full, merge node with sibling
        else {
            parent->left->large = parent->small;
            parent->small.clear();
            parent->middle = nullptr;
            delete node;
        }
    }

    //if node is a right child
    else { //HAVENT IMPLEMENTED
        throw runtime_error("Haven't implemented right child merge.");
    }

    //if parent is root, set merged node to root
    if (parent == root && parent->small.empty() && parent->large.empty()){
        root = parent->left;
        root->parent = nullptr;
        delete parent;
    }
}

void Tree::reorderTwoKeys(Node* node){
    //if only small key exists, return
    if (!node->small.empty() && node->large.empty()){
        return;
    }
    //if large key exists without small key, reorder
    else if (node->small.empty() && !node->large.empty()){
        node->small = node->large;
        node->large.clear();
    }
    //if small key is larger than large key, reorder
    else if (node->small > node->large){
        const string changeValue = node->small;
        node->small = node->large;
        node->large = changeValue;
    }
}

string Tree::reorderThreeKeys(Node* node, const string& value){
    string keyToMoveUp = value;

    //small key moves up to parent
    if (value < node->small){
        keyToMoveUp = node->small;
        node->small = value;
    }
    //large key moves up to parent
    else if (value > node->large){
        keyToMoveUp = node->large;
        node->large = value;
    }
    //if the inserting key is between small and large key, move up inserting key to parent
    return keyToMoveUp;
}

Node* Tree::prepareOperation(const string& value) const{
    Node* curr = root;

    while (curr){
        //traverse left child
        if (value < curr->small && curr->left){
            curr = curr->left;
        }
        //traverse middle child
        else if (value > curr->small && (value < curr->large || curr->large.empty()) && curr->middle){
            curr = curr->middle;
        }
        //traverse right child
        else if (value > curr->large && curr->right){
            curr = curr->right;
        }
        //node found
        else {
            return curr;
        }
    }
    throw runtime_error("Value not found.");
}

void Tree::preOrder(Node* node) const {
    if (!node) return;

    cout << node->small << ", ";
    preOrder(node->left);
    if (!node->large.empty()){
        cout << node->large << ", ";
    }
    preOrder(node->middle);
    preOrder(node->right);
}

void Tree::inOrder(Node* node) const {
    if (!node) return;

    inOrder(node->left);
    cout << node->small << ", ";
    inOrder(node->middle);
    if (!node->large.empty()){
        cout << node->large << ", ";
    }
    inOrder(node->right);
}

void Tree::postOrder(Node* node) const {
    if (!node) return;

    postOrder(node->left);
    postOrder(node->middle);
    cout << node->small << ", ";
    postOrder(node->right);  
    if (!node->large.empty()){
        cout << node->large << ", ";
    }
}