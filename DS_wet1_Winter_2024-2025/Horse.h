#pragma once
#include "ProjectFiles.h"
//#include "Herd.h"
#include <cassert>
#define NULL_ID (-1)
//#include "HorseList.h"
#include "LinkedList.h"

class Herd; //added as forward declaration due to circular inclusion

class Horse { //: public IndexAble<Horse>, public TraceAble<Horse>{
private:
    friend class HorseList;
    int horseId;
    int speed;
    int herdID;
    int herdInsertions;
    Horse* follows;
    int followsInsertion;
    bool special_bool;
    Herd* herd;
    Node<Horse>* thisLink;
public:
    void setHerd(Herd* herd) {
        this->herd = herd;
    }
    void setLink(Node<Horse>* link) {
        this->thisLink = link;
    }
    int getID() const{
        return this->horseId;
    }
    int getHerdID(){
        return this->herdID;
    }


    Horse(int id, int speed) : horseId(id), speed(speed), herdID(NULL_ID), herdInsertions(0),
                               follows(nullptr), followsInsertion(0), special_bool(false), herd(nullptr),
                                thisLink(nullptr){}
    ~Horse(){
        this->follows = nullptr;
        this->herd = nullptr;
        this->thisLink = nullptr;
    }
    int getSpeed() const{ return this->speed;}
    void setFollow(int horseToFollow);
    void setHerd(int herdId);
// comparison operators overload for use in tree methods.
    bool operator==(const Horse& otherHorse) const;
    bool operator>(const Horse& otherHorse) const;
    bool operator<(const Horse& otherHorse) const;
    bool independant(){
        Horse* followedHorse = this->follows;
        if (followedHorse == nullptr){return true;}
        if (!(this->sameHerd(followedHorse))){return true; }
        int otherHerdInsertions = followedHorse->herdInsertions;
        //if the horse that 'this' follows has moved a herd since
        //'this' started following it.
        if (this->followsInsertion != otherHerdInsertions){return true;}
        assert((this->followsInsertion == otherHerdInsertions)&&(this->herdID == followedHorse->herdID)); //FIXME might be a source of a problem here or with leave / join herd
        return false;
    }

    inline bool sameHerd(Horse* otherHorse){return this->herdID == otherHorse->herdID;}

    bool follow(Horse* leader){
        if((this->herd == nullptr)||(!(this->sameHerd(leader)))){ //fixme same non herd horses somehow pass this condition
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
        bool checkedNext = (maybeLeader)?true:this->follows->alreadyChecked();
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
    bool leaveHerd();

    Herd* getHerd() {
        return this->herd;
    }

    void join_herd(Herd* herd);
//    void join_herd(Herd* herd){
//        this->herdID = herd->getID();
//        ++(this->herdInsertions);
//    }
};