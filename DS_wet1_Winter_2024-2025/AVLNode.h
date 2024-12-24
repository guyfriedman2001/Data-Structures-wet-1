#pragma once
#include <cassert>
#define NULL_ID (-1)
#include  <iostream>
#include "AVL.h"
#define EMPTY_TREE_HEIGHT 0

using std::cout;

template <typename Value>
class AVLNode {
protected:
    int index;
    Value* value;
    AVLNode<Value>* left;
    AVLNode<Value>* right;
    int height;

public:

    enum class Roll {
        noRoll,
        LL,
        LR,
        RL,
        RR
    };

    AVLNode(Value& value)
        : index(value.getId()), value(value), left(nullptr), right(nullptr), height(EMPTY_TREE_HEIGHT + 1){}

    virtual ~AVLNode() {
        delete left;
        delete right;
        delete value;
    }



protected:
    friend class AVL;
    void insertRight(AVLNode<Value>* node) {
        if (!this->right) {
            this->right = node;
        } else {
            this->right = this->right->insert(node);
        }
    }

    void insertLeft(AVLNode<Value>* node) {
        if (!this->left) {
            this->left = node;
        } else {
            this->left = this->left->insert(node);
        }
    }

    /**
     * this function will be used for debugging in AVL class
     * for example - assert(heightVerified()) after inserting or deleting
     * values
     */
    bool heightVerified(AVLNode<Value>* node){
        if (node == nullptr){
            return true;
        }
        bool leftTree = heightVerified(node->left);
        bool rightTree = heightVerified(node->right);
        int oldHeight = node->height;
        int newHeight = node->heightUpdate();
        bool thisNode = (oldHeight == newHeight);
        return leftTree&&rightTree&&thisNode;
    }

    /**
     * same as above
     */
    bool isBalanced(AVLNode<Value>* node){
        if (node == nullptr){
            return true;
        }
        bool leftTree = isBalanced(node->left);
        bool rightTree = isBalanced(node->right);
        int nodesBalance = node->balanceFactor();
        bool thisNode = ((-1<=nodesBalance) && (nodesBalance<=1));
        return leftTree&&rightTree&&thisNode;
    }



    AVLNode<Value>* Balance() {
        this->heightUpdate(); //make sure height is updated
        Roll roll = this->getRoll();
        switch (roll) {
            case Roll::noRoll:
                return this;
            case Roll::LL:
                return this->LL();
            case Roll::LR:
                return this->LR();
            case Roll::RL:
                return this->RL();
            case Roll::RR:
                return this->RR();
            default:
                throw "Problem in Balance switch case";
        }
    }

    /**
     * update the height value,
     * this function is going to be called many times, sometimes seemingly unnecesairly,
     * but it is done in order to account for inhereting classes overriding some methods and forgetting to call this function
     */
    int heightUpdate() {
        int leftHeight = this->left ? this->left->height : EMPTY_TREE_HEIGHT;
        int rightHeight = this->right ? this->right->height : EMPTY_TREE_HEIGHT;
        this->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
        return this->height;
    }

    int balanceFactor() const {
        this->heightUpdate(); //make sure height is updated
        int leftHeight = this->left ? this->left->height : EMPTY_TREE_HEIGHT;
        int rightHeight = this->right ? this->right->height : EMPTY_TREE_HEIGHT;
        return leftHeight - rightHeight;
    }

    Roll getRoll() const {
        this->heightUpdate(); //make sure height is updated
        int balance = this->balanceFactor();
        if (-1 <= balance && balance <= 1) return Roll::noRoll;
        if (balance > 1) {
            return this->left->balanceFactor() >= 0 ? Roll::LL : Roll::LR;
        }
        return this->right->balanceFactor() <= 0 ? Roll::RR : Roll::RL;
    }

    AVLNode<Value>* LL() {
        AVLNode<Value>* temp = this->left;
        this->left = temp->right;
        temp->right = this;
        return temp;
    }

    AVLNode<Value>* LR() {
        this->left = this->left->RR();
        return this->LL();
    }

    AVLNode<Value>* RL() {
        this->right = this->right->LL();
        return this->RR();
    }

    AVLNode<Value>* RR() {
        AVLNode<Value>* temp = this->right;
        this->right = temp->left;
        temp->left = this;
        return temp;
    }

    inline bool isLeaf(){
        return (this->left == nullptr) && (this->right == nullptr);
    }

    inline bool oneChild(){
        return (this->left == nullptr) ^ (this->right == nullptr);
    }

    inline bool twoChildern(){
        return (this->left != nullptr) && (this->right != nullptr);
    }

    /**
     * absorb a given node into 'this', effectively 'deleting' 'this'.
     */
    void absorbNode(AVLNode<Value>* nodeToAbsorb){
        assert(!this->isLeaf());
        this->index = nodeToAbsorb->index;
        delete this->value;
        this->value = nodeToAbsorb->value;
        nodeToAbsorb->value = nullptr;
        delete nodeToAbsorb;
        this->heightUpdate(); //extra call
    }

    /**
     * absorb the child of the node.
     */
    void absorbChild(){
        assert(this->oneChild());
        AVLNode<Value>* child = nullptr;
        if (this->left != nullptr){
            child = this->left;
        } else {
            child = this->right;
        }
        this->left = this->right = nullptr;
        this->absorbNode(child);
    }

    /**
     * replace the value of 'this' with its succesor in the in-order order.
     */
    void replaceWithSuccssessor(){
        assert(this->twoChildern());
        AVLNode<Value>* succssesor = nullptr;
        if (this->right->left == nullptr){
            succssesor = this->right;
            this->right = nullptr;
        } else {
        succssesor = this->right->getSmallest();
        }
        int succssesorIndex = succssesor->index;
        this->absorbNode(succssesor);
        this->right = this->right->updateLeftPath();
        this->heightUpdate(); //extra call
    }

    /**
     * get the value with the smallest index of a given tree,
     * notice this function leaves the tree unorganized, calling functions must organize afterwards
     * using the updatePath function
     * 
     * @return - the value with the smallest index of a given tree
     */
    AVLNode<Value>* getSmallest() {
        assert(this->left != nullptr);
        if(this->left->left != nullptr){
            return this->left->getSmallest();
        }
        AVLNode<Value>* temp = this->left;
        this->left = temp->right;
        assert(temp->left == nullptr);
        return temp;
    }

    /**
     * update path along an index.
     * 
     * @return - the head of the balanced sub tree
     */
    AVLNode<Value>* updatePath(int index){ //FIXME function takes O(n) time!
        int thisIndex = this->index;
        int fixIndex = index;
        assert(thisIndex != fixIndex);
        if (fixIndex < thisIndex){
            this->left = (this->left == nullptr)?  nullptr : this->left->updatePath(fixIndex);
        } else {
            this->right = (this->right == nullptr)?  nullptr : this->right->updatePath(fixIndex);
        }
        this->heightUpdate();
        return this->Balance();
    }

        /**
     * update path along an index.
     * 
     * @return - the head of the balanced sub tree
     */
    AVLNode<Value>* updateLeftPath(){ //FIXME function takes O(n) time!
        if (this->left == nullptr){
            return this->Balance();
        }
        this->left = this->left->updateLeftPath();
        return this->Balance();
    }

    AVLNode<Value>* deleteThis() { //return the sub tree of 'this' without the node of 'this'.
        delete this->value;
        if (this->isLeaf()) {
            delete this;
            return nullptr;
        }
        if (this->oneChild){
            this->absorbChild();
        }
        if (this->twoChildern()){
            this->replaceWithSuccssessor();
        }
        this->heightUpdate();
        return this->Balance();
    }


public:

    /**
     * wrapping function to overload insert, same functionality
     * as inserting a an existing node / sub tree.
     * provides extra functionality.
     * 
     * @param value - value of the node to be inserted.
     * @return - the root of the balanced tree after the addition of the new value.
     */
    AVLNode<Value>* insert(Value& value) { //removed const, we are deleting the value after runtime
        AVLNode<Value>* insertThis = new (std::nothrow) AVLNode<Value>(value);
        if (!insertThis){throw StatusType::ALLOCATION_ERROR;}
        return this->insert(&insertThis);
    }
    
    /**
     * insert a new node into the tree of 'this', return the balanced tree.
     * 
     * @param node - the node to be inserted into the tree of 'this'
     * @return - the balanced tree of 'this' after insertion
     */
    AVLNode<Value>* insert(AVLNode<Value>* node) {
        int nodeIndex = node->index;
        int thisIndex = this->index;

        if (nodeIndex == thisIndex) {
            cout << "inserted node with same index as existing node";
            delete node;
            return this;
        }

        if (nodeIndex < thisIndex) {
            this->insertLeft(node);
        } else {
            this->insertRight(node);
        }
        this->heightUpdate();
        return this->Balance();
    }

    /**
     * wrapping function to overload deleteNode, provide extra functionality in case
     * a reference for the value to be deleted already exists.
     * 
     * @param value - value to be deleted/ removed from the tree.
     * @return - the balanced tree without the removed value.
     */
    AVLNode<Value>* deleteNode(Value& value) {
        int toDelete = value.getId();
        return this->deleteNode(toDelete);
    }

    /**
     * remove a node from the tree of 'this', return the balanced tree, with returnVal updated at
     * the head of the tree to contain the appropriate value.
     * 
     * value may or may not be deleted as well, subject to the definition of 'deleteValue()'.
     * 
     * @param index - the index of the node to be removed.
     * @return - the balanced tree after the removal of said node.
     */
    AVLNode<Value>* deleteNode(int index) {
        if (index == this->index) {
            return this->deleteThis();
        }

        if (index < this->index) {
            if (!this->left) {
                return this;
            }
            this->left = this->left->deleteNode(index);
        } else {
            if (!this->right) {
                return this;
            }
            this->right = this->right->deleteNode(index);
        }
        this->heightUpdate();
        return this->Balance();
    }


    /**
     * return a pointer to a value by a given index,
     * if value is not found (index not in the tree) returned pointer will be null,
     * however for the case that the value held at that index is supposed to be null,
     * returnValue handling is encased in a specified function - findNotFound().
     * 
     * @param index - the index of the value to retrieve its pointer.
     * @return - a pointer to the value of the corresponding index.
     */
    Value* find(int index){
            if (index == this->index) {
            return this->value;
        }
        Value* searchedValue = nullptr;
        if (index < this->index) {
            if (!this->left) {
                return nullptr;
            }
            searchedValue = this->left->find(index);
        } else {
            if (!this->right) {
                return nullptr;
            }
            searchedValue = this->right->find(index);
        }
        return searchedValue;
    }
};


