#include <cassert>

template <typename Derived, typename V, typename R>
class AbstractAVL {
protected:
    static constexpr int EMPTY_TREE_HEIGHT = 0;
    static constexpr int MINUS_INFINITY = INT64_MIN;
    int index;
    V* value;
    Derived<V, R>* left;
    Derived<V, R>* right;
    Derived<V, R>* parent; // added to allow inorder travel without allocating extra memory
    int height;
    R* returnVal; // This should be null (or equivalent) unless actively holding a required return value

public:
//    iterator begin() {
//        return iterator(this);
//    }
//    iterator end() {
//        return iterator(nullptr);
//    }

    enum class Roll {
        noRoll,
        LL,
        LR,
        RL,
        RR
    };

    AbstractAVL(V& value)
        : index(value.getId()), value(value), left(nullptr), right(nullptr), height(EMPTY_TREE_HEIGHT + 1), returnVal(nullptr) {}

    virtual ~AbstractAVL() {
        delete left;
        delete right;
        deleteValue();
        deleteReturnVal();
    }

    /**
     * return a new node of an instance of the inhereting class
     * 
     * @param value - the value to be contained in the node
     * @return - a node of an instance of the inhereting class avl with the supplied value
     */
    virtual Derived<V, R>* returnNewInstance(V& value) = 0;

    /**
     * define the desired behavior of the tree when a node with
     * an existing value / index is given.
     * 'this' in this case would refer to the node that is already in the tree containing the same index.
     * desired data should be stored in 'this', since otherNode will be deleted straight after this function is called.
     * returnVal could be stored in either nodes.
     * 
     * @param otherNode - the node that was trying to be inserted. 
     */
    virtual void sameIndex(Derived<V, R>* otherNode) = 0;

    /**
     * @return - desired value indicating a sucssesful insertion
     */
    virtual R* insertionSucces() = 0;

    /**
     * @return - desired value indicating a sucssesful deletion
     */
    virtual R* deleteSucces() = 0;

    /**
     * @return - desired value indicating that a deleted value was not found
     */
    virtual R* deleteNotFound() = 0;

    /**
     * FOR OWNING TREES ONLY - add call to the destructor of value
     * FOR NON OWNING TREES - do nothing
     */
    virtual void deleteValue() = 0;

    /**
     * how to handle deletion of returnVal
     */
    virtual void deleteReturnVal() = 0;

    /**
     * this function decides what happens when no return value is instantiated
     * 
     * default behaviour - no return value found is bad
     */
    virtual void noReturnValFound(){
        throw "Problem in 'updateReturnVal' - no return value instantiated";
    }

    /**
     * new function must provide default implementation
     */
    virtual R* getReturnVal(){
        return nullptr;
    }



protected:

    void insertRight(Derived<V, R>* node) {
        if (!this->right) {
            this->right = node;
            this->returnVal = insertionSucces();
        } else {
            this->right = this->right->insert(node);
            this->returnVal = this->right->returnVal;
        }
    }

    void insertLeft(Derived<V, R>* node) {
        if (!this->left) {
            this->left = node;
            this->returnVal = insertionSucces();
        } else {
            this->left = this->left->insert(node);
            this->returnVal = this->left->returnVal;
        }
    }

    void updateReturnVal(Derived<V, R>* node) {
        if (this->returnVal) return;
        if (node->returnVal) {
            this->returnVal = node->returnVal;
            node->returnVal = nullptr; // Reset node's returnVal
            return;
        }
        //if we got to this line, either there is an extra call to this function, or returnval was not instantiated correctly.
        bool noReturnValFound = true;
        assert(!noReturnValFound);
        //noReturnValFound();
    }

    Derived<V, R>* Balance() {
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

    Derived<V, R>* LL() {
        Derived<V, R>* temp = this->left;
        this->left = temp->right;
        temp->right = this;
        temp->updateReturnVal(this);
        return temp;
    }

    Derived<V, R>* LR() {
        this->left = this->left->RR();
        return this->LL();
    }

    Derived<V, R>* RL() {
        this->right = this->right->LL();
        return this->RR();
    }

    Derived<V, R>* RR() {
        Derived<V, R>* temp = this->right;
        this->right = temp->left;
        temp->left = this;
        temp->updateReturnVal(this);
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
    void absorbNode(Derived<V, R>* nodeToAbsorb){
        assert(!this->isLeaf());
        this->deleteValue{};
        this->index = nodeToAbsorb->index;
        this->value = nodeToAbsorb->value;
        nodeToAbsorb->value = nullptr;
        delete nodeToAbsorb;
        this->heightUpdate(); //extra call
        //NOTICE NO returnVal CREATED HERE!
    }

    /**
     * absorb the child of the node.
     */
    void absorbChild(){
        assert(this->oneChild());
        Derived<V, R>* child = nullptr;
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
        Derived<V, R>* succssesor = nullptr;
        if (this->right->left == nullptr){
            succssesor = this->right;
            this->right = nullptr;
        } else {
        succssesor = this->right->getSmallest();
        }
        int succssesorIndex = succssesor->index;
        this->absorbNode(succssesor);
        this->right = this->right->updatePath(succssesorIndex);
        this->heightUpdate(); //extra call
    }

    /**
     * get the value with the smallest index of a given tree,
     * notice this function leaves the tree unorganized, calling functions must organize afterwards
     * using the updatePath function
     * 
     * @return - the value with the smallest index of a given tree
     */
    Derived<V, R>* getSmallest() {
        assert(this->left != nullptr);
        if(this->left->left != nullptr){
            return this->left->getSmallest();
        }
        Derived<V, R>* temp = this->left;
        this->left = temp->right;
        assert(temp->left == nullptr);
        return temp;
    }

    /**
     * update path along an index.
     * 
     * @return - the head of the balanced sub tree
     */
    Derived<V, R>* updatePath(int index){
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

    Derived<V, R>* deleteThis() { //return the sub tree of 'this' without the node of 'this'.
        this->deleteValue();
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
        this->returnVal = deleteSucces();
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
    Derived<V, R>* insert(const V& value) {
        Derived<V, R>* node = returnNewInstance(value);
        return this->insert(node);
    }
    
    /**
     * insert a new node into the tree of 'this', return the balanced tree.
     * 
     * @param node - the node to be inserted into the tree of 'this'
     * @return - the balanced tree of 'this' after insertion
     */
    Derived<V, R>* insert(Derived<V, R>* node) {
        int nodeIndex = node->index;
        int thisIndex = this->index;

        if (nodeIndex == thisIndex) {
            this->sameIndex(node);
            this->updateReturnVal(node);
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
    Derived<V, R>* deleteNode(V& value) {
        return this->deleteNode(value.getId());
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
    Derived<V, R>* deleteNode(int index) {
        if (index == this->index) {
            return this->deleteThis();
        }

        if (index < this->index) {
            if (!this->left) {
                this->returnVal = deleteNotFound();
                return this;
            }
            this->left = this->left->deleteNode(index);
        } else {
            if (!this->right) {
                this->returnVal = deleteNotFound();
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
    V* find(int index){
            if (index == this->index) {
            return this->value;
        }
        V* searchedValue = nullptr;
        if (index < this->index) {
            if (!this->left) {
                this->returnVal = findNotFound();
                return nullptr;
            }
            searchedValue = this->left->find(index);
            this->updateReturnVal(this->left);
        } else {
            if (!this->right) {
                this->returnVal = findNotFound();
                return nullptr;
            }
            searchedValue = this->right->find(index);
            this->updateReturnVal(this->right);
        }
        return searchedValue;
    }
};

/**
 *
 * added Itertor class for inorder travel
 * changes made- added parent pointer to AbstractAVL structure,
 * we now need to make sure to properly set a parent in the tree methods (insert and delete)
 */
//template <typename Derived, typename V, typename R>
//class AbstractAVL::Iterator{
//    AbstractAVL* current;
//
//    // finds the left most node (minimal value);
//    void FindLeft(AbstractAVL* node) {
//        while (node && node->left) {
//            node = node->left;
//        }
//        return node;
//    }
//    // finds the next node in inorder travel
//    AbstractAVL* findNext(AbstractAVL* node) {
//        if (node->right) {
//            // If there is a right subtree, the next node is the leftmost node in that subtree
//            return findLeft(node->right);
//        }
//        // Otherwise, move up to the parent until the node is a left child
//        AbstractAVL* parent = node->parent;
//        while (parent && node == parent->right) {
//            node = parent;
//            parent = parent->parent;
//        }
//        return parent;
//    }
//
//public:
//    iterator(AbstractAVL* root = nullptr) : current(findLeft(root)) {}
//    V& operator*() const {
//        return current->value;
//    }
//    iterator& operator++() {
//        current = findNext(current);
//        return *this;
//    }
//    bool operator==(const iterator& other) const {
//        return current == other.current;
//    }
//    bool operator!=(const iterator& other) const {
//        return !(*this == other);
//    }
//};
