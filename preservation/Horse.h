#pragma once
#include "ProjectFiles.h"
//#include "Herd.h"
#include <cassert>
#define NULL_ID (-1)

class Herd; //added as forward declaration due to circular inclusion

class Horse { //: public IndexAble<Horse>, public TraceAble<Horse>{
private:
    int horseId;
    int speed;
    int herdID;
    int herdInsertions;
    Horse* follows;
    int followsInsertion;
    bool special_bool;
public:
    int getID() const{
        return this->horseId;
    }
    int getHerdID(){
        return this->herdID;
    }
    Horse(int id, int speed) : horseId(id), speed(speed), herdID(NULL_ID), herdInsertions(0),
    follows(nullptr), followsInsertion(0), special_bool(false){}
    ~Horse() = default;
    int getSpeed() const{ return this->speed;}
    void setFollow(int horseToFollow);
    void setHerd(int herdId);
// comparison operators overload for use in tree methods.
    bool operator==(const Horse& otherHorse) const;
    bool operator>(const Horse& otherHorse) const;
    bool operator<(const Horse& otherHorse) const;
    bool independant(){
        Horse* followedHorse;
        if ((followedHorse = this->follows) == nullptr){return true;}
        int otherHerdInsertions = followedHorse->herdInsertions;
        //if the horse that 'this' follows has moved a herd since
        //'this' started following it.
        if (this->followsInsertion != otherHerdInsertions){return true;}
        assert((this->followsInsertion == otherHerdInsertions)&&(this->herdID == followedHorse->herdID));
        return false;
    }

    inline bool sameHerd(Horse* otherHorse){return this->herdID == otherHorse->herdID;}

    bool follow(Horse* leader){
        if(!(this->sameHerd(leader))){
            return false;
        }
        this->follows = leader;
        this->followsInsertion = leader->herdInsertions;
        return true;
    }

    Horse* getFollows();
    inline bool alreadyChecked(){return this->special_bool;}
    inline void markChecked(){this->special_bool = true;}
    inline void unCheck(){this->special_bool = false;}

    /**
     * NOTICE - THIS FUNCTION DOES NOT PRESERVE special_bool == false,
     * RESET BOOL **MUST** BE INVOKED ON HERD AFTER THIS FUNCTION IS CALLED
     */
    bool inCircularReferance(int jumps){
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
     * return true if left herd,
     * false if was not in a herd
     */
    bool leaveHerd(){
        if (this->herdID == NULL_ID){
            return false;
        }
        this->herdID = NULL_ID;
        ++(this->herdInsertions);
        return true;
    }

    void join_herd(Herd* herd);
//    void join_herd(Herd* herd){
//        this->herdID = herd->getID();
//        ++(this->herdInsertions);
//    }
};