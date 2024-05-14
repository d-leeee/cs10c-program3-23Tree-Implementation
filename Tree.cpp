#include "Tree.h"
#include <iostream>

using namespace std;

void Tree::insert(const string& value){
    //empty tree
    if (empty()){
        root = new Node(value);
    }

    Node* curr = root;
    Node* parent = nullptr;
    while (curr){
        //left child
        if (value < curr->small && curr->left){
            curr = curr->left;
        }
        //middle child
        else if (value < curr->large && curr->middle){
            curr = curr->middle;
        }
        //right child
        else if (value > curr->large && curr->right){
            curr = curr->right;
        }
        //no child
        else {
            //if large key is empty
            if (curr->large == ""){
                curr->large = value;
                reorderTwoKeys(curr);
                return;
            }
            //if large key is full
            string keyToMoveUp = reorderThreeKeys(curr, value); //order small, large, and new keys
            split(curr, keyToMoveUp, nullptr, nullptr); //move keyToMoveUp to parent node and split the tree
        }
    }
}

void Tree::remove(const string& value){

}

bool Tree::search(const string& value) const {

}

void Tree::preOrder(Node* node) const {
}

void Tree::inOrder(Node* node) const {
    
}

void Tree::postOrder(Node* node) const {

    
}

void Tree::reorderTwoKeys(Node* node){
    if (node->small > node->large){
        const string changeValue = node->small;
        node->small = node->large;
        node->large = changeValue;
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

void Tree::split(Node* node, const string& keyToMoveUp, Node* leftNode, Node* rightNode){
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
}