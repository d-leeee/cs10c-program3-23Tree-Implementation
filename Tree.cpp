#include "Tree.h"
#include <iostream>

using namespace std;

// Insert a new value into the tree
void Tree::insert(const string& value){
    // If empty tree
    if (empty()){
        root = new Node(value);
        return;
    }

    Node* insertPlace = prepareOperation(value); // Node to insert value into

    // If large key is empty
    if (insertPlace->large.empty()){
        insertPlace->large = value;
        reorderTwoKeys(insertPlace);
    }
    // If large key is full
    else {
        string keyToMoveUp = reorderThreeKeys(insertPlace, value); //order small, large, and new keys
        split(insertPlace, keyToMoveUp);                           //move keyToMoveUp to parent node and split the tree
    }
}

// Remove a value from the tree
void Tree::remove(const string& value){
    //return error if value DNE (HAVEN'T IMPLEMENTED YET)
    if (!search(value)){
        throw runtime_error("Value does not exist.");
    }

    Node* victim = prepareOperation(value); // Node to delete value from

    // CASE 1: If value is in leaf node
    if (!victim->left && !victim->middle && !victim->right){
        // If the value is equal to small key
        if (victim->small == value) {
            // If value is single value node
            if (victim->large.empty()){ 
                // If node is root, update parent pointers
                if (victim == root){
                    delete root;
                    root = nullptr;
                }
                // If node is not root, merge
                else {
                    merge(victim);
                }
            }
            // If value is two value node
            else { 
                victim->small.clear();
                reorderTwoKeys(victim);
            }
        }
        // If the value is equal to large key
        else { 
            victim->large.clear();
        }
    }

    // CASE 2: If value is not a leaf node
    else {
        // Look for a successor
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

        // Replace value by successor value
        if (victim->small == value){
            victim->small = successor->small;
        }
        else {
            victim->large = successor->small;
        }
        reorderTwoKeys(victim);

        // If successor only has one value, merge
        if (successor->large.empty()){
            merge(successor);
        }
        // If successor has two values, remove small key
        else {
            successor->small.clear();
            reorderTwoKeys(successor);
        }    
    }
}

// Search for the existance of a value in the tree
bool Tree::search(const string& value) const {
    throw runtime_error("Haven't implemented search function");
    return false;
}

// Split a node into 2 and move a node to the parent (used by insert function)
void Tree::split(Node* node, const string& keyToMoveUp){
    Node* leftNode = new Node(node->small);  // Small node
    Node* rightNode = new Node(node->large); // Large node

    // If node is root
    if (node == root){
        root = new Node(keyToMoveUp);
        root->left = leftNode;
        root->middle = rightNode;
        leftNode->parent = root;
        rightNode->parent = root;
    }
    //If node's parent is not full
    else if (node->parent->large.empty()){
        node->parent->large = keyToMoveUp;
        node->parent->middle = leftNode;
        node->parent->right = rightNode;
        reorderTwoKeys(node->parent);
    }
    delete node;
}

// Merge 2 nodes into 1 by bringing a parent down (used by remove function)
void Tree::merge(Node* node){
    Node* parent = node->parent;

    // If node is a left child
    if (parent->left == node){
        // If sibling is full, move parent small to node, siblings large to parent
        if (!parent->middle->large.empty()){
            node->small = parent->small;
            parent->small = parent->middle->small;
            parent->middle->small.clear();
            reorderTwoKeys(parent->middle);
        }
        // If sibling is not full, merge node with sibling
        else {
            node->small = parent->small;
            node->large = parent->middle->small;
            parent->small.clear();
            parent->middle = nullptr;
            reorderTwoKeys(node);
            delete parent->middle;
        }
    }

    // If node is a middle child
    else if (parent->middle == node){
        // If sibling is full, move parent small to node, siblings large to parent
        if (!parent->left->large.empty()){
            node->small = parent->small;
            parent->small = parent->left->large;
            parent->left->large.clear();
        }
        // If sibling is not full, merge node with sibling
        else {
            parent->left->large = parent->small;
            parent->small.clear();
            parent->middle = nullptr;
            delete node;
        }
    }

    // If node is a right child (HAVEN'T IMPLEMENTED YET)
    else {
        throw runtime_error("Haven't implemented right child merge.");
    }

    // If parent is root, set merged node to root
    if (parent == root && parent->small.empty() && parent->large.empty()){
        root = parent->left;
        root->parent = nullptr;
        delete parent;
    }
}

// Reorder a node's keys from smallest to largest (used when changing a node's keys)
void Tree::reorderTwoKeys(Node* node){
    // If only small key exists, return
    if (!node->small.empty() && node->large.empty()){
        return;
    }
    // If large key exists without small key, reorder
    else if (node->small.empty() && !node->large.empty()){
        node->small = node->large;
        node->large.clear();
    }
    // If small key is larger than large key, reorder
    else if (node->small > node->large){
        const string changeValue = node->small;
        node->small = node->large;
        node->large = changeValue;
    }
}

// When a node is full and needs to insert, reorder the node and return the key to move up to parent (used by insert function)
string Tree::reorderThreeKeys(Node* node, const string& value){
    string keyToMoveUp = value;

    // Small key moves up to parent
    if (value < node->small){
        keyToMoveUp = node->small;
        node->small = value;
    }
    // Large key moves up to parent
    else if (value > node->large){
        keyToMoveUp = node->large;
        node->large = value;
    }
    // If the inserting key is between small and large key, move up inserting key to parent
    return keyToMoveUp;
}

// Search for the node to access and mutate (used by search, insert, remove functions)
Node* Tree::prepareOperation(const string& value) const{
    Node* curr = root;

    while (curr){
        // Traverse left child
        if (value < curr->small && curr->left){
            curr = curr->left;
        }
        // Traverse middle child
        else if (value > curr->small && (value < curr->large || curr->large.empty()) && curr->middle){
            curr = curr->middle;
        }
        // Traverse right child
        else if (value > curr->large && curr->right){
            curr = curr->right;
        }
        // Node found
        else {
            return curr;
        }
    }
    throw runtime_error("Value not found.");
}

// Recursively print preorder
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

// Recursively print inorder
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

// Recursively print postorder
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