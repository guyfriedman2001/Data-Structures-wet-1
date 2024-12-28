#pragma once
#include  "Pair.h"
#include  "Horse.h"
#define NULL_ID (-1)
#include "ProjectFiles.h"
#include  <new>

#include "Horse.h"
#include "Horse.h"
#include "Horse.h"


class HorseNode {
    friend class HorseList;
    Horse* data;
    HorseNode* next;
    HorseNode* previous;

public:
    explicit HorseNode(Horse* data = nullptr, HorseNode* next = nullptr, HorseNode* previous = nullptr)
            : data(data), next(next), previous(previous) {}
    ~HorseNode(){
        delete next;
        previous = nullptr;
        data = nullptr;
    };

    HorseNode* getNext() {
        return this->next;
    }
    HorseNode* getPrevious() {
        return this->previous;
    }
    Horse* getData() {
        return this->data;
    }
    void setNext(HorseNode* newNext) {
        this->next = newNext;
    }
    void setPrevious(HorseNode* newPrevious) {
        this->previous = newPrevious;
    }
    void removeYourself() { //delete the node after calling it
        this->previous->next = this->next;
        this->next->previous = this->previous;
        this->next = nullptr;
        this->previous = nullptr;
        this->data = nullptr;
    }
};

class HorseList{
private:
    friend class Horse; 
    HorseNode* head;
    int length;
public:
    HorseList() : head(nullptr){}

    /**
     *
    ~LinkedList() {
        Node<T>* current = head;
        while (current) {
            Node<T> *next = current->getNext();
            delete current;
            current = next;
        }
    }
     */

    ~HorseList() = default;
    bool insert(Horse* type){
        HorseNode* newnode = new (std::nothrow) HorseNode(&type);
        if (!newnode) {
            throw StatusType::ALLOCATION_ERROR;
        }

        type->setLink(newnode);

        newnode->setNext(this->head);
        if (this->head != nullptr){
            this->head->setPrevious(newnode);
        }
        this->head = newnode;

        return true;
    }

    bool remove(Horse* type) {
        assert(type->herd == this);
        type->leaveHerd(); //TODO UPDATE LEAVEHERD IN HORSE\
        --(this->length);
    }


    class Iterator {
        HorseNode* current;
    public:
        explicit Iterator(HorseNode* node = nullptr) : current(node) {}
        Horse& operator*() const {
            return *(current->getData());
        }
        Iterator& operator++() {
            if (current) current = current->getNext();
            return *this;
        }
        Iterator& operator--() {
            if (current) current = current->previous;
            return *this;
        }
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    Iterator begin() {
        return Iterator(head);
    }
    Iterator end() {
        return Iterator(nullptr);
    }
};
