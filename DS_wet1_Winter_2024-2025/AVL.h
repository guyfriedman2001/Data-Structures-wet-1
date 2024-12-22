#pragma once
#include <cassert>
#include "AVLNode.h"

template <typename Value>
class AVL {
protected:
    AVLNode<Value>* head;

public:
    virtual AVLNode(V& value):head(AVLNode<Value>(value)){}
    virtual AVLNode():head(nullptr){}
    virtual ~AVLNode(){
        delete head;
    }

    bool insert(Value& value){
        int valueIndex = value.getID();
        if (this->head == nullptr){
            this->head = new AVLNode(Value);
            return true;
        }
        Value* exists = this->head->find(valueIndex);
        if (exists != nullptr){
            return false;
        }
        this->head = this->head->insert(value);
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
        return true;
    }

    Value* get(int index){
        if (this->head == nullptr){
            return nullptr;
        }
        return this->head->find(index);
    }

};

