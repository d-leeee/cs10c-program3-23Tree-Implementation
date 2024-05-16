#include "Tree.h"
#include <iostream>

using namespace std;

void Tree::insert(const string& value){
    //empty tree
    if (empty()){
        root = new Node(value);
        return;
    }

    Node* curr = root;
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
            split(curr, keyToMoveUp); //move keyToMoveUp to parent node and split the tree
            return;
        }
    }
}

void Tree::remove(const string& value){
    //return error if value dne
    if (!search(value)){
        throw runtime_error("Value does not exist.");
    }
    Node* curr = root;
    while (curr){
        //traverse left
        if (value < curr->small){
            curr = curr->left;
        }
        //traverse middle
        else if (value < curr->large){
            curr = curr->middle;
        }
        //traverse right
        else if (value > curr->large){
            curr = curr->right;
        }
        //value found
        else {
            //if node is full
            if (curr->large != ""){
                //if value is small key, delete small and reorder
                if (value == curr->small){
                    curr->small = "";
                    reorderTwoKeys(curr);
                }
                //if value is large key, just remove large key
                else if (value == curr->large){
                    curr->large = "";
                }
                return;
            }
            //if node is empty
            Node* deleteNode = curr;
            //if value is at root node
            if (value == root->small){
                root = nullptr;
            }
            //if value is a right child and left sibling exists
            else if (curr->small > curr->parent->small && curr->parent->left){
                //if parent is not full, move sibling to parent
                if (curr->parent->large == ""){
                    curr->parent->large = curr->parent->left->small;
                    delete curr->parent->left;
                    //reorder parent
                    reorderTwoKeys(curr);
                }
            }
            //if value is a left child and right sibling exists
            else if (curr->small < curr->parent->small && curr->parent->right){
                //if parent is not full, move sibling to parent
                if (curr->parent->large == ""){
                    curr->parent->large = curr->parent->right->small;
                    delete curr->parent->right;
                    //reorder parent
                    reorderTwoKeys(curr);
                }
            }
            //delete the value
            delete deleteNode;
            return;
        }
    }
}

bool Tree::search(const string& value) const {
    return true;
}

void Tree::preOrder(Node* node) const {
    if (!node){
        return;
    }

    cout << node->small << ", ";

    preOrder(node->left);
    preOrder(node->middle);
    preOrder(node->right);

    if (node->large != ""){
        cout << node->large << ", ";
    }
}

void Tree::inOrder(Node* node) const {
    if (!node){
        return;
    }

    inOrder(node->left);

    cout << node->small << ", ";

    inOrder(node->middle);

    if (node->large != ""){
        cout << node->large << ", ";
    }

    inOrder(node->right);
}

void Tree::postOrder(Node* node) const {
    if (!node){
        return;
    }

    postOrder(node->left);
    postOrder(node->middle);
    postOrder(node->right);  

    cout << node->small << ", ";
    if (node->large != ""){
        cout << node->large << ", ";
    }
}

void Tree::reorderTwoKeys(Node* node){
    if (node->small == "" && node->large != ""){
        node->small = node->large;
        node->large = "";
    }
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

void Tree::split(Node* node, const string& keyToMoveUp){
    Node* leftNode = new Node(node->small);
    Node* rightNode = new Node(node->large);
    //if node is root
    if (node == root){
        root = new Node(keyToMoveUp);
        root->left = leftNode;
        root->middle = rightNode;
    }
    //if node's parent is not full
    else if (node->parent->large == ""){
        node->parent->large = keyToMoveUp;
        node->parent->middle = leftNode;
        node->parent->right = rightNode;
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