#pragma once
#include "ProjectFiles.h"
#include <cassert>

//this tree does not have ownership of the horses, but rather only
//provides a way to find them by their index.


class HorseMap : public AbstractAVL<HorseMap,Horse,StatusType>{

public:

    HorseMap(Horse& horse) : AbstractAVL(horse){}

    StatusType* getReturnVal() override {
        return this->returnVal;
    }


    /**
     * return a new node of an instance of the inhereting class
     * 
     * @param value - the value to be contained in the node
     * @return - a node of an instance of the inhereting class avl with the supplied value
     */
     HorseMap* returnNewInstance(Horse& value) override {
        return new HorseMap(value); // Adjust constructor call as necessary
    }

    /**
     * define the desired behavior of the tree when a node with
     * an existing value / index is given.
     * 'this' in this case would refer to the node that is already in the tree containing the same index.
     * desired data should be stored in 'this', since otherNode will be deleted straight after this function is called.
     * returnVal could be stored in either nodes.
     * 
     * @param otherNode - the node that was trying to be inserted. 
     */
    virtual void sameIndex(AbstractAVL<AbstractAVL,Horse, StatusType>* otherNode) override {
        this->returnVal = output_t(StatusType::FAILURE);
    }


    /**
     * @return - desired value indicating a sucssesful insertion
     */
    virtual StatusType insertionSucces() override {
        return output_t(StatusType::SUCCESS);
    }

    /**
     * @return - desired value indicating a sucssesful deletion
     */
    virtual StatusType deleteSucces() override {
        return output_t(StatusType::SUCCESS);
    }

    /**
     * @return - desired value indicating that a deleted value was not found
     */
    virtual StatusType deleteNotFound() override {
        return output_t(StatusType::FAILURE);
    }

    /**
     * FOR OWNING TREES ONLY - add call to the destructor of value
     * FOR NON OWNING TREES - do nothing
     */
    virtual void deleteValue() override {
        //this tree does not own the data,
        //therefor this function does nothing.

        return;
    }

    /**
     * how to handle deletion of returnVal
     */
    virtual void deleteReturnVal() override {
        //TODO do we need to take care of destruction of status type?
        //for now - do nothing.

        return;
    }

    /**
     * this function decides what happens when no return value is instantiated
     * 
     * default behaviour - no return value found is bad
     */
    virtual void noReturnValFound(){
        throw "Problem in 'updateReturnVal' - no return value instantiated";
    }

    /**
     * count how many horses in our tree are independant (follows no one)
     * counting done in a post order manner
     * 
     * @return - ammount of horses that dont follow anyone
     */
    virtual int countIndependant(){
        int leftIndependant = this->countLeftIndependant();
        int rightIndependant = this->countRightIndependant();
        int thisIndependance = (this->value->independant())?1:0;
        return leftIndependant + rightIndependant + leftIndependant;
    }



    /**
     * find if there is a circular referencing within our database,
     * utilise Horse::special_bool to avoid excessive checks.
     */
    bool circularReferencing(){
        bool leftTree = ((this->left==nullptr)?false:this->left->circularReferencing(this->height));
        if (leftTree){
            //if the left tree has a circular referencing, no need to continue checking.
            return true;
        }
        bool rightTree = ((this->right == nullptr)?false:this->right->circularReferencing(this->height));
        if (rightTree){
            //if the right tree has a circular referencing, no need to continue checking.
            return true;
        }
        bool answer = this->circularReferencing(this->height);
        this->resetBoolean();
        return answer;
    }

    //protected:

    /**
     * reset the boolean value of special_bool inside the horses back to its default value of false
     */
    void resetBoolean(){
        if(this->left != nullptr){
            this->left->resetBoolean();
        }
        if (this->right != nullptr){
            this->right->resetBoolean();
        }
        this->value->unCheck();
    }

    /**
     * helper function to reduce ammount of code inside countIndependant().
     * 
     * @return - ammount of horses that 'follow no one'.
     */
    int countLeftIndependant(){
        if (this->left == nullptr){
            return 0;
        }
        return this->left->countIndependant();
    }

    /**
     * helper function to reduce ammount of code inside countIndependant().
     */
    int countRightIndependant(){
        if (this->right == nullptr){
            return 0;
        }
        return this->right->countIndependant();
    }

    bool circularReferencing(int originalHeight){
        if (this->checkLeftCircle(originalHeight)){
            //if the left tree has a circular referencing
            return true;
        }
        if (this->checkRightCircle(originalHeight)){
            //if the right tree has a circular referencing
            return true;
        }
        return this->checkValueCircular(originalHeight);
    }

    bool checkLeftCircle(int originalHeight){
        if (this->left == nullptr){
            return false;
        }
        return this->left->circularReferencing(originalHeight);
    }

    bool checkRightCircle(int originalHeight){
        if (this->right == nullptr){
            return false;
        }
        return this->right->circularReferencing(originalHeight);
    }

    /**
     * find if there is a circular referencing within our database,
     * utilise Horse::special_bool to avoid excessive checks.
     * 
     * NOTICE - THIS FUNCTION DOES NOT PRESERVE THE STATE OF Horse::special_bool == false
     *  CALLERS TO THIS FUNCTION MUST FOLLOW WITH A CALL TO RESET_BOOL
     */
    bool checkValueCircular(int treeSize){
        return this->value->inCircularReferance(treeSize);
    }

    bool can_run_together(){
        //check if the whole map contains horses that can run together
        int potentialLeaders = this->countIndependant();
        if (potentialLeaders!=1){return false;}
        this->resetBoolean(); //extra precaution
        bool circular = this->circularReferencing();
        //if there is no circular referencing and only one leader, the horses can run together.
        return !this->circularReferencing();
    }






};