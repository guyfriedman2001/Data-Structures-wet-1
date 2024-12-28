#pragma once
#define NULL_ID (-1)
#include  <new>
#include "ProjectFiles.h"

template <typename T>
class Node {
    //friend class LinkedList;
    public:
    T* data;
    Node<T>* next;
    Node<T>* previous;

public:
    explicit Node(T* data = nullptr, Node<T>* next = nullptr, Node<T>* previous = nullptr)
            : data(data), next(next), previous(previous) {}
    ~Node(){
        delete next;
        previous = nullptr;
        data = nullptr;
    };

    Node<T>* getNext() {
        return this->next;
    }
    Node<T>* getPrevious() {
        return this->previous;
    }
    T* getData() {
        return this->data;
    }
    void setNext(Node<T>* newNext) {
        this->next = newNext;
    }
    void setPrevious(Node<T>* newPrevious) {
        this->previous = newPrevious;
    }
    void removeYourself() {
        this->next->previous = this->previous;
        this->previous->next = this->next;
        this->next = nullptr;
        this->previous = nullptr;
        this->data = nullptr;
        delete this;
    }
};

template <typename T>
class LinkedList{
private:
    Node<T>* head;
    Node<T>* tail;

public:
    LinkedList() {
        this->head = new (std::nothrow) Node<T>();
        this->tail = new (std::nothrow) Node<T>();
        if (this->head == nullptr || this->tail == nullptr) {
            throw StatusType::ALLOCATION_ERROR;
        }
        this->head->setNext(this->tail);
        this->tail->setPrevious(this->head);
    }

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

    ~LinkedList() = default;
    Node<T>* getHead() {
        return this->head->getNext();
    }
    bool insert(T* type){
        Node<T>* newnode = new (std::nothrow) Node<T>(type);
        if (!newnode) {
            throw StatusType::ALLOCATION_ERROR;
        }
        this->head->next->previous = newnode;
        newnode->setNext(this->head->getNext());
        this->head->setNext(newnode);
        newnode->setPrevious(this->head);

        return true;
    }

    Node<T>* getFirst() {
        return this->head->getNext();
    }

    Node<T>* getLast() {
        return this->tail;
    }

    bool remove(T* type){
        //for (Node<T> node : this->head){  ====== attempts to iterate over this->head directly, which is not iterable ======
        for (Iterator it = begin(); it != end(); it++) {
            Node<T>* node = it.current;

            if (node->data != type){continue;}
            if (node->previous != nullptr){
                node->previous->next = node->next;
            }
            if (node->next != nullptr){
                node->next->previous = node->previous;
            }
            node->data = nullptr; //linked list should not delete held data
            delete node; //====== changed from delete &node ======
            return true;
        }
        return false;
    }

    class Iterator {
        Node<T>* current;
    public:
        explicit Iterator(Node<T>* node = nullptr) : current(node) {}
        T& operator*() {
            return *(current->getData());
        }
        Iterator& operator++() {
            if (current) current = current->getNext();
            return *this;
        }
//        Iterator& operator--() {
//            if (current) current = current->previous;
//            return *this;
//        }
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    Iterator begin() {
        return Iterator(this->head->getNext());
    }
    Iterator end() {
        return Iterator(this->tail->getPrevious());
    }
};
