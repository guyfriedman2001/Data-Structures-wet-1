#pragma once

template <typename implementing_class>
/**
 * this class provides an interface for graphs with 'one next node',
 * the functionality of this interface is to check if there is a chain greater than a certaing limit,
 * or if there is a circular referencing within the graph.
 */
class TraceAble{
private:
    bool special_bool;
public:
    virtual ~TraceAble() = default;
    virtual TraceAble() : special_bool(false){}
    virtual bool hasNext() = 0;
    bool isChecked(){return this->special_bool;}
    void markChecked(){this->special_bool = true;}
    void unCheck(){this->special_bool = false;}

    /**
     * if there is no 'next', return nullptr.
     */
    virtual implementing_class* getNext() = 0;

    /**
     * NOTICE - THIS FUNCTION DOES NOT PRESERVE special_bool == false,
     * RESET BOOL **MUST** BE INVOKED ON HERD AFTER THIS FUNCTION IS CALLED
     */
    bool chainLessThan(int jumps){ //FIXME - move parameters from horses to abstract TraceAble
        if (jumps < 0){return true;}
        bool maybeLeader = this->independant();
        bool checkedNext = this->follows->alreadyChecked();
        if (maybeLeader||checkedNext){
            this->markChecked();
            return false;
        }
        bool checkNext = this->follows->inCircularReferance(--jumps);
        this->markChecked();
        return checkNext;
    }

    /**
     * this functions purpose is to serve the next assignment and courses,
     * since it is easier to use but less efficient.
     * 
     * check the graph for circular referencing.
     * 
     * @return - true if in circular referencing, otherwise false.
     */
    bool circularReferencing(){}
};