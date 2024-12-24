#define NULL_ID (-1)


template <typename T>
class Node {
    T* data;
    Node<T>* next;
    Node<T>* previous;

public:
    explicit Node(T* data = nullptr, Node<T>* next = nullptr, Node<T>* previous = nullptr)
            : data(data), next(next), previous(previous) {}
    ~Node(){
        data = nullptr;
        //delete data; //made this into a comment, because the linked list should not delete its members
    };

    Node<T>* getNext() {
        return this->next;
    }
    Node<T>* getPrevious() {
        return this->previous;
    }
};

template <typename T>
class LinkedList{
private:
    Node<T>* head;
public:
    LinkedList() : head(nullptr){}
    ~LinkedList() {
        Node<T>* current = head;
        while (current) {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }
    }
    bool insert(T* type){
        Node<T> newnode = new (std::nothrow) Node<T>(type);
        if (!newnode){throw StatusType::ALLOCATION_ERROR;}
        newnode->next = this->head;
        if (this->head != nullptr){
            this->head->previous = newnode;
        }
        this->head = newnode;
        return true;
    }

    bool remove(T* type){
        for (Node<T> node : this->head){
            if (node->data != type){continue;}
            if (node->previous != nullptr){
                node->previous->next = node->next;
            }
            if (node->next != nullptr){
                node->next->previous = node->previous;
            }
            node->data = nullptr; //linked list should not delete held data
            delete &node;
            return true;
            
        }
        return false;
    }

    class Iterator {
        Node<T>* current;
    public:
        explicit Iterator(Node<T>* node = nullptr) : current(node) {}
        T& operator*() const {
            return *(current->data);
        }
        Iterator& operator++() {
            if (current) current = current->next;
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
