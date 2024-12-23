template <typename T>
class LinkedList{
private:
    Node<T>* head;
public:
    LinkedList(T* type):head(type){}
    ~LinkedList(){
        delete head;
    }
    bool insert(T* type){
        Node<T> newnode = new (std::nothrow) Node<T>(type);
        if (!newnode){
            return false;
        }
        newnode->next = this->head;
        if (this->head != nullptr){
            this->head->previous = &newnode;
        }
        this->head = &newnode;
        return true;
    }

    bool remove(T* type){
        return false;
    }

    /**
     * make the node iterator callable from the LinkedList
     */
    class Iterator;
    Iterator begin() {
        Node<T>* temp = this;
        while (temp && temp->previous) {
            temp = temp->previous;
        }
        return Iterator(temp);
    }
    Iterator end() {
        Node<T>* temp = this;
        while (temp && temp->next) {
            temp = temp->next;
        }
        //should return a pointer to the null value after the last node
        return Iterator(temp->next);
    }
};


template <class T>
class Node {
    T* data;
    Node<T>* next;
    Node<T>* previous;

public:
    explicit Node(T* data = nullptr, Node<T>* next = nullptr, Node<T>* previous = nullptr)
            : data(data), next(next), previous(previous) {}
    ~Node() = default;

    Node<T>* getNext() {
        return this->next;
    }
    Node<T>* getPrevious() {
        return this->previous;
    }

    class Iterator;
    Iterator begin() {
        Node<T>* temp = this;
        while (temp && temp->previous) {
            temp = temp->previous;
        }
        return Iterator(temp);
    }
    Iterator end() {
        Node<T>* temp = this;
        while (temp && temp->next) {
            temp = temp->next;
        }
        //should return a pointer to the null value after the last node
        return Iterator(temp->next);
    }
};

template <class T>
class Node<T>::Iterator {
    Node<T>* current;
public:
    explicit Iterator(Node* node = nullptr) : current(node) {}
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
