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
        cout << "empty, inserting " << value << " into " << insertPlace->small << " node." << endl;
    }
    else {
        //if large key is full
        string keyToMoveUp = reorderThreeKeys(insertPlace, value); //order small, large, and new keys
        split(insertPlace, keyToMoveUp); //move keyToMoveUp to parent node and split the tree
        cout << "full, moving " << keyToMoveUp << " to parent. " << endl;
    }
}

void Tree::remove(const string& value){
    //return error if value DNE
    if (!search(value)){
        throw runtime_error("Value does not exist.");
    }

    Node* removePlace = prepareOperation(value);
    cout << "The node containing the value to be removed has " << removePlace->small << ", " << removePlace->large << endl;

    //CASE 1: if value is in leaf node
    if (!removePlace->left && !removePlace->middle && !removePlace->right){
        cout << "value is inside a leaf node." << endl;
        //if the value is equal to small key
        if (removePlace->small == value) {
            cout << "value is equal to small key" << endl;
            //single value node
            if (removePlace->large.empty()){ 
                cout << "the node does not contain a large key, removing the node" << endl;
                //if node is not root, update parent pointers
                if (removePlace != root){
                    cout << "node is not a root node" << endl;
                    if (removePlace->parent->left == removePlace) {
                        cout << "node is a left child, removing left child" << endl;
                        removePlace->parent->left = nullptr;
                    } 
                    else if (removePlace->parent->middle == removePlace){
                        cout << "node is a middle child, removing middle child" << endl;
                        removePlace->parent->middle = nullptr;
                    }
                    else {
                        cout << "node is a right child, removing right child" << endl;
                        removePlace->parent->right = nullptr;
                    }
                    delete removePlace;
                }
                //if node is root, delete root
                else {
                    cout << "node is a root node, deleting root" << endl;
                    delete root;
                    root = nullptr;
                    return;
                }
            }
            //two value node
            else { 
                cout << "the node contains a large key, keeping the node and removing small key" << endl;
                removePlace->small.clear();
                reorderTwoKeys(removePlace);
                cout << "the node is now " << removePlace->small << endl;
            }
        }
        //if the value is equal to large key
        else { 
            cout << "value is equal to large key, deleting large key" << endl;
            removePlace->large.clear();
            cout << "node is now " << removePlace->small << endl;
        }
    }

    //CASE 2: if value is not a leaf node
    else {
        cout << "value is not a leaf node, looking for successor" << endl;
        //look for a successor
        Node* successor; 
        if (removePlace->small == value){
            successor = removePlace->middle;
        }
        else {
            successor = removePlace->right;
        }
        while (successor->left){
            successor = successor->left;
        }
        cout << "successor found, " << successor->small << endl;
        //replace value by successor value
        if (removePlace->small == value){
            cout << "value is a small key, replacing small key with " << successor->small << endl;
            removePlace->small = successor->small;
        }
        else {
            cout << "value is a large key, replacing large key with " << successor->small << endl;
            removePlace->large = successor->small;
        }
        reorderTwoKeys(removePlace);
        cout << "the node is now " << removePlace->small << ", " << removePlace->large << endl;

        //if successor only has one value, merge
        if (successor->large.empty()){
            cout << "successor only has one value, needs to merge" << endl;
            Node* parent = successor->parent;
            cout << "successor's parent is " << parent->small << ", " << parent->large << endl;
            //if sibling is full, move parent small to successor, siblings large to parent
            if (!parent->left->large.empty()){
                cout << "sibling is " << parent->left->small << ", " << parent->left->large << endl;
                cout << "sibling is full, moving parent small key to successor and sibling's large key to parent small" << endl;
                successor->small = parent->small;
                parent->small = parent->left->large;
                parent->left->large.clear();
                cout << "successor is now " << successor->small << ", " << successor->large << endl;
                cout << "sibling is now " << parent->left->small << ", " << parent->left->large << endl;
                cout << "parent is now " << parent->small << ", " << parent->large << endl;
            }
            //if sibling is not full, merge successor with sibling
            else {
                cout << "sibling is " << parent->left->small << ", " << parent->left->large << endl;
                cout << "sibling is not full, needs to merge successor with sibling" << endl;
                parent->left->large = parent->small;
                parent->small.clear();
                cout << "sibling is now " << parent->left->small << ", " << parent->left->large << endl;
                cout << "parent is now " << parent->small << ", " << parent->large << endl;
                if (parent->middle == successor){
                    cout << "successor is a middle child, deleting middle child" << endl;
                    parent->middle = nullptr;
                }
                else {
                    cout << "successor is a right child, deleting right child" << endl;
                    parent->right = nullptr;
                }
                delete successor;
            }
            //if parent is root, set merged node to root
            if (parent == root && parent->small.empty() && parent->large.empty()){
                cout << "successor parent is root node and is empty, setting new root to merged node" << endl;
                cout << "the merged node becoming root is " << parent->left->small << ", " << parent->left->large << endl;
                root = parent->left;
                root->parent = nullptr;
                delete parent;
            }
        }
        //if successor has two values, remove small key
        else {
            cout << "successor has two values, removing small key" << endl;
            successor->small.clear();
            reorderTwoKeys(successor);
            cout << "successor is now " << successor->small << ", " << successor->large << endl;
        }      
    }
}

bool Tree::search(const string& value) const {
    return true;
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
        cout << "parent does not exist. Moving " << keyToMoveUp << " to root. Root's left children are " << root->left->small << ", " << root->left->large
         << " and Root's middle children are " << root->middle->small << ", " << root->middle->large << endl;
    }
    //if node's parent is not full
    else if (node->parent->large.empty()){
        node->parent->large = keyToMoveUp;
        node->parent->middle = leftNode;
        node->parent->right = rightNode;
        reorderTwoKeys(node->parent);
        cout << "parent is not full, parent is now " << node->parent->small << ", " << node->parent->large << ". Parent's middle children are "
        << node->parent->middle->small << ", " << node->parent->middle->large << " and Parent's right children are " << node->parent->right->small << ", " << node->parent->right->large << endl;
    }
    delete node;
}

void Tree::merge(Node* node){

}

void Tree::reorderTwoKeys(Node* node){
    if (!node->small.empty() && node->large.empty()){
        return;
    }
    else if (node->small.empty() && !node->large.empty()){
        node->small = node->large;
        node->large.clear();
    }
    else if (node->small > node->large){
        const string changeValue = node->small;
        node->small = node->large;
        node->large = changeValue;
    }
}

string Tree::reorderThreeKeys(Node* node, const string& value){
    string keyToMoveUp = value;
    if (value < node->small){
        keyToMoveUp = node->small;
        node->small = value;
    }
    else if (value > node->large){
        keyToMoveUp = node->large;
        node->large = value;
    }
    else{
        keyToMoveUp= value;
    }
    return keyToMoveUp;
}

Node* Tree::prepareOperation(const string& value) const{
    Node* curr = root;
    while (curr){
        //left child
        if (value < curr->small && curr->left){
            curr = curr->left;
            cout << "traversing left" << endl;
        }
        //middle child
        else if (value > curr->small && (value < curr->large || curr->large.empty()) && curr->middle){
            curr = curr->middle;
            cout << "traversing middle" << endl;
        }
        //right child
        else if (value > curr->large && curr->right){
            curr = curr->right;
            cout << "traversing right" << endl;
        }
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