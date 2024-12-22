template <class T>
class Node {
    T* data;
    Node* next;
    Node* previous;

public:
    Node(T* data = nullptr, Node* next = nullptr, Node* previous = nullptr)
    : data(data), next(next), previous(previous) {}

    Iterator begin() {
        Node* temp = this;
        while (temp && temp->previous) {
            temp = temp->previous;
        }
        return Iterator(temp);
    }
    Iterator end() {
        Node* temp = this;
        while (temp && temp->next) {
            temp = temp->next;
        }
        //should return a pointer to the null value after the last node
        return Iterator(temp->next);
    }
};

class Node:Iterator {
    Node* current;
public:
    Iterator(Node* node = nullptr) : current(node) {}
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
