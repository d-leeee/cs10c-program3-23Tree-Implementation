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
    if (insertPlace->large == ""){
        insertPlace->large = value;
        reorderTwoKeys(insertPlace);
        //cout << "empty, inserting " << value << " into " << curr->small << " node." << endl;
    }
    else {
        //if large key is full
        string keyToMoveUp = reorderThreeKeys(insertPlace, value); //order small, large, and new keys
        split(insertPlace, keyToMoveUp); //move keyToMoveUp to parent node and split the tree
        //cout << "full, moving " << keyToMoveUp << " to parent. " << endl;
    }
}

void Tree::remove(const string& value){
    //return error if value DNE
    if (!search(value)){
        throw runtime_error("Value does not exist.");
    }

    Node* removePlace = prepareOperation(value);
    //if value is in leaf node
    if (!removePlace->left){
        if (removePlace->small == value) {
            if (removePlace->large == ""){
                if (removePlace != root) {
                removePlace->parent->left = nullptr;
                    delete removePlace;
                } 
                else {
                    delete root;
                    root = nullptr;
                    return;
                }
            }
            else {
                removePlace->small = "";
            }
        }
        else {
            removePlace->large = "";
        }
        reorderTwoKeys(removePlace);
    }
    //if value is internal node
    else {
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
        //replace value by successor value
        if (removePlace->small == value){
            removePlace->small = successor->small;
        }
        else {
            removePlace->large = successor->small;
        }
        reorderTwoKeys(removePlace);

        //if successor only has one value, merge
        if (successor->large == ""){
            successor->parent->left->large = successor->parent->small;
            successor->parent->middle = nullptr;
            root = successor->parent->left;
            root->parent = nullptr;
        }
    }

    /*
    //if node is full
    if (removePlace->large != ""){
        //if value is small key, delete small and reorder
        if (value == removePlace->small){
            removePlace->small = "";
            reorderTwoKeys(removePlace);
        }
        //if value is large key, just remove large key
        else if (value == removePlace->large){
            removePlace->large = "";
        }
    }
    //if node is not full
    else {
        Node* deleteNode = removePlace;
        //if value is at root node
        if (removePlace == root){
            delete root;
            root = nullptr;
            return;
        }
        //if value is a left child
        if (removePlace->parent->left == removePlace){
            removePlace->parent->left = nullptr;
        }
        //if value is a middle child
        else if (removePlace->parent->middle == removePlace){
            removePlace->parent->middle = nullptr;
        }
        //if value is a right child
        else {
            removePlace->parent->right = nullptr;
        }



        //delete the value
        delete deleteNode;
    }
    */
}

bool Tree::search(const string& value) const {
    return true;
}

Node* Tree::prepareOperation(const string& value) const{
    Node* curr = root;
    while (curr){
        //left child
        if (value < curr->small && curr->left){
            curr = curr->left;
            //cout << "traversing left" << endl;
        }
        //middle child
        else if ((curr->large == "" || value < curr->large) && curr->middle){
            curr = curr->middle;
            //cout << "traversing middle" << endl;
        }
        //right child
        else if (value > curr->large && curr->right){
            curr = curr->right;
            //cout << "traversing right" << endl;
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
    if (node->large != ""){
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
    if (node->large != ""){
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
    if (node->large != ""){
        cout << node->large << ", ";
    }
}

void Tree::reorderTwoKeys(Node* node){
    if (node->small == "" && node->large != ""){
        node->small = node->large;
        node->large = "";
        //cout << "small is empty, moving large to small" << endl;
    }
    else if (node->small > node->large){
        const string changeValue = node->small;
        //cout << "swapping " << node->small << ", " << node->large << " to ";
        node->small = node->large;
        node->large = changeValue;
        //cout << node->small << ", " << node->large << endl;
    }
}

string Tree::reorderThreeKeys(Node* node, const string& value){
    reorderTwoKeys(node);
    string keyToMoveUp = value;
    if (value < node->small){
        keyToMoveUp = node->small;
        node->small = value;
    }
    else if (value > node->large){
        keyToMoveUp = node->large;
        node->large = value;
    }
    return keyToMoveUp;
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
        /*cout << "parent does not exist. Moving " << keyToMoveUp << " to root. Root's left children are " << root->left->small << ", " << root->left->large
         << " and Root's middle children are " << root->middle->small << ", " << root->middle->large << endl;*/
    }
    //if node's parent is not full
    else if (node->parent->large == ""){
        node->parent->large = keyToMoveUp;
        node->parent->middle = leftNode;
        node->parent->right = rightNode;
        reorderTwoKeys(node->parent);
        /*cout << "parent is not full, parent is now " << node->parent->small << ", " << node->parent->large << ". Parent's middle children are "
        << node->parent->middle->small << ", " << node->parent->middle->large << " and Parent's right children are " << node->parent->right->small << ", " << node->parent->right->large << endl;*/
    }
    delete node;
    /*
    string newKeyToMoveUp = reorderThreeKeys(node, keyToMoveUp);
    Node* smallNode = new Node(node->small); 
    Node* largeNode = new Node(node->large);
    //if parent doesn't exist
    if (node == root){
        root = new Node(newKeyToMoveUp);
        root->left = smallNode;
        root->right = largeNode;
    }
    //if parent node large key is full
    else if (node->parent->large != ""){
        //left split
        if (leftNode->small < node->small && rightNode->small < node->small){
            smallNode->left = leftNode;
            smallNode->middle = rightNode;
            largeNode->left = node->middle;
            largeNode->middle = node->right;
        }
        //middle split
        else if (leftNode->small < node->large && rightNode->small < node->large){
            smallNode->left = node->left;
            smallNode->middle = leftNode;
            largeNode->left = rightNode;
            largeNode->middle = node->right;
        }
        //right split
        else {
            smallNode->left = node->left;
            smallNode->middle = node->middle;
            largeNode->left = leftNode;
            largeNode->middle = rightNode;
        }
        split(node->parent, newKeyToMoveUp, smallNode, largeNode);
    }
    //if parent node large key is empty
    else if (node->parent->large == ""){
        node->parent->large = newKeyToMoveUp;
        reorderTwoKeys(node->parent);
        //left split
        if (node->small > smallNode->small){
            node->left = smallNode;
            node->right = node->middle;
            node->middle = largeNode;
        }
        //right split
        else {
            node->middle = leftNode;
            node->right = rightNode;
        }
    }
    */
}