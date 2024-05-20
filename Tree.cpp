#include "Tree.h"
#include <iostream>

using namespace std;

// Insert a new key into the tree
void Tree::insert(const string& key) {

    // If empty tree
    if (empty()) {
        root = new Node(key);
        return;
    }

    Node* insertNode = prepareOperation(key); // Node to insert key into

    // CASE 1: If key already exists
    if (insertNode->small == key || insertNode->large == key) {
        throw runtime_error("key already exists.");
    }

    // CASE 2: If large key is empty
    else if (insertNode->large.empty()) {
        insertNode->large = key;
        reorderTwoKeys(insertNode);
    }

    // CASE 3: If large key is full
    else {
        string keyToMoveUp = reorderThreeKeys(insertNode, key);   // Find the key to move up to parent
        split(insertNode, keyToMoveUp);                           // Move keyToMoveUp to parent node and split the tree
    }
}

// Remove a key from the tree
void Tree::remove(const string& key) {

    Node* victim = prepareOperation(key); // Node to delete key from

    // CASE 1: If key is in leaf node
    if (!victim->left && !victim->middle && !victim->right) {

        // If the key is equal to small key
        if (victim->small == key) {

            // If key is single key node
            if (victim->large.empty()) { 

                // If node is root, update parent pointers
                if (victim == root) {
                    delete root;
                    root = nullptr;
                }

                // If node is not root, merge
                else {
                    merge(victim);
                }
            }

            // If key is two key node
            else { 
                victim->small.clear();
                reorderTwoKeys(victim);
            }
        }

        // If the key is equal to large key
        else { 
            victim->large.clear();
        }
    }

    // CASE 2: If key is not a leaf node
    else {

        // Look for a successor subtree
        Node* successor; 
        if (victim->small == key) {
            successor = victim->middle;
        }
        else {
            successor = victim->right;
        }

        // Traverse subtree to find successor node
        while (successor->left) {
            successor = successor->left;
        }

        // Replace victim key by successor key
        if (victim->small == key) {
            victim->small = successor->small;
        }
        else {
            victim->large = successor->small;
        }
        reorderTwoKeys(victim);

        // If successor only has one key, merge
        if (successor->large.empty()) {
            merge(successor);
        }

        // If successor has two keys, remove small key
        else {
            successor->small.clear();
            reorderTwoKeys(successor);
        }    
    }
}

// Search for the existance of a key in the tree
bool Tree::search(const string& key) const {
    throw runtime_error("Haven't implemented search function");
    return false;
}

// Split a node into 2 and move a node to the parent (used by insert function)
void Tree::split(Node* node, const string& keyToMoveUp) {

    Node* leftNode = new Node(node->small);    // Small node
    Node* rightNode = new Node(node->large);   // Large node
    Node* parent = node->parent;               // Parent node

    // CASE 1: If node is root
    if (node == root) {
        root = new Node(keyToMoveUp);
        root->left = leftNode;
        root->middle = rightNode;
        leftNode->parent = root;
        rightNode->parent = root;
    }

    // CASE 2: If node's parent is not full
    else if (parent->large.empty()) {
        parent->large = keyToMoveUp;
        parent->middle = leftNode;
        parent->right = rightNode;
        reorderTwoKeys(parent);
    }

    // CASE 3: If node's parent is full
    else if (!parent->large.empty()) {
        throw runtime_error("Haven't implemented recursive insert yet.");
    }

    delete node;
}

// Merge 2 nodes into 1 by bringing a parent down to node
void Tree::merge(Node* node) {

    Node* parent = node->parent;

    // CASE 1: If node is a left child
    if (parent->left == node) {

        // If sibling is full, move parent's small key to node, sibling's large to parent
        if (!parent->middle->large.empty()) {
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
    
    // CASE 2: If node is a middle child
    else if (parent->middle == node) {
        // If sibling is full, move parent small to node, siblings large to parent
        if (!parent->left->large.empty()) {
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

    // CASE 3: If node is a right child (HAVEN'T IMPLEMENTED YET)
    else {
        throw runtime_error("Haven't implemented right child merge.");
    }

    // If parent is root, set merged node to root
    if (parent == root && parent->small.empty() && parent->large.empty()) {
        root = parent->left;
        root->parent = nullptr;
        delete parent;
    }
}

// Reorder a node's keys from smallest to largest (used when changing a node's keys)
void Tree::reorderTwoKeys(Node* node) {

    // If only small key exists, return
    if (!node->small.empty() && node->large.empty()) {
        return;
    }

    // If large key exists without small key, reorder
    else if (node->small.empty() && !node->large.empty()) {
        node->small = node->large;
        node->large.clear();
    }

    // If small key is larger than large key, reorder
    else if (node->small > node->large) {
        const string changekey = node->small;
        node->small = node->large;
        node->large = changekey;
    }
}

// When a node is full and needs to insert, reorder the node and return the key to move up to parent (used by insert function)
string Tree::reorderThreeKeys(Node* node, const string& key /* Inserted key */ ) {

    string keyToMoveUp = key;

    // Small key moves up to parent
    if (key < node->small) {
        keyToMoveUp = node->small;
        node->small = key;
    }

    // Large key moves up to parent
    else if (key > node->large) {
        keyToMoveUp = node->large;
        node->large = key;
    }

    // If the inserting key is between small and large key, move up inserted key to parent
    return keyToMoveUp;
}

// Search for the node to access and mutate (used by search, insert, remove functions)
Node* Tree::prepareOperation(const string& key) const {

    Node* curr = root;

    // Traverse the tree to find key
    while (curr) {

        // Traverse left child
        if (key < curr->small && curr->left) {
            curr = curr->left;
        }

        // Traverse middle child
        else if (key > curr->small && (key < curr->large || curr->large.empty()) && curr->middle) {
            curr = curr->middle;
        }

        // Traverse right child
        else if (key > curr->large && curr->right) {
            curr = curr->right;
        }

        // Node found
        else {
            return curr;
        }
    }

    throw runtime_error("key not found.");
}

// Recursively print preorder
void Tree::preOrder(Node* node) const {

    if (!node) return;

    cout << node->small << ", ";
    preOrder(node->left);     // Traverse left subtree

    if (!node->large.empty()) {
        cout << node->large << ", ";
    }

    preOrder(node->middle);   // Traverse middle subtree
    preOrder(node->right);    // Traverse right subtree
}

// Recursively print inorder
void Tree::inOrder(Node* node) const {

    if (!node) return;

    inOrder(node->left);     // Traverse left subtree
    cout << node->small << ", ";
    inOrder(node->middle);   // Traverse middle subtree

    if (!node->large.empty()) {
        cout << node->large << ", ";
    }

    inOrder(node->right);    // Traverse right subtree
}

// Recursively print postorder
void Tree::postOrder(Node* node) const {

    if (!node) return;

    postOrder(node->left);     // Traverse left subtree
    postOrder(node->middle);   // Traverse middle subtree
    cout << node->small << ", ";
    postOrder(node->right);    // Traverse right subtree

    if (!node->large.empty()) {
        cout << node->large << ", ";
    }
}