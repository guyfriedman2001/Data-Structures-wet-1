#pragma once
#include <cassert>
#include "AVLNode.h"
#define NULL_ID -1


template <typename Value>
class AVL {
protected:
    AVLNode<Value>* head;

public:
    virtual AVL(Value& value):head(AVLNode<Value>(value)){} //FIXME
    virtual AVL():head(nullptr){}
    virtual ~AVL(){
        delete head;
    }

    bool insert(Value& value){
        int valueIndex = value.getID();
        if (this->head == nullptr){
            this->head = new AVLNode(Value);
            if (!this->head){throw StatusType::ALLOCATION_ERROR;}
            return true;
        }
        Value* exists = this->head->find(valueIndex);
        if (exists != nullptr){
            return false;
        }
        this->head = this->head->insert(value);
        assert(this->verifyTree());
        return true;
    }

    bool remove(int index){
        if (this->head == nullptr){
            return false;
        }
        Value* exists = this->head->find(index);
        if (exists == nullptr){
            return false;
        }
        this->head = this->head->deleteNode(index);
        assert(this->verifyTree());
        return true;
    }

    Value* get(int index){
        assert(this->verifyTree());
        if (this->head == nullptr){
            return nullptr;
        }
        return this->head->find(index);
    }

    /**
     * this function will be used for debugging
     */
    bool verifyTree(){
        bool heightVerified = AVLNode::heightVerified(this->head);
        bool balanceVerified = AVLNode::isBalanced(this->head);
        return heightVerified && balanceVerified;
    }

};

