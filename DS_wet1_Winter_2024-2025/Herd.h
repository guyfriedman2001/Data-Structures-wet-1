#pragma once
#include "ProjectFiles.h"
#include "LinkedList.h"
#define NULL_ID (-1)

class Herd {
private:
    int herdId;
    LinkedList<Horse> herdMembers;
    int totalMembers;
public:
    explicit Herd(int id):herdId(id), herdMembers(), totalMembers(0){}
    ~Herd() = default;
    int getID() const{
        return this->herdId;
    }
    // comparison operators overload for use in tree methods.
    bool operator==(const Herd& otherHerd) const;
    bool operator>(const Herd& otherHerd) const;
    bool operator<(const Herd& otherHerd) const;

    bool add_horse(Horse* horse){
        assert(addHorseAssertHelper(horse));
        herdMembers.insert(horse);
        ++(this->totalMembers);
        horse->join_herd(this);
        horse->setLink(this->herdMembers.getHead());
        return true;
    }

    bool addHorseAssertHelper(Horse* horse) {
        Node<Horse>* curr = this->herdMembers.getHead();
        Node<Horse>* last = this->herdMembers.getLast();
        while (curr != last) {
            Horse* horsy = curr->getData();
            if (horsy == horse) {
                return false;
            }
            curr = curr->getNext();
        }
        return true;
    }

    void leave() {
        --(this->totalMembers);
    }
    
    bool leads(int followerID, int leaderID){
        assert((followerID != leaderID)&&(followerID>0)&&(leaderID>0));
        Horse* follower = nullptr;
        Horse* leader = nullptr;
        Node<Horse>* curr = this->herdMembers.getHead();
        Node<Horse>* last = this->herdMembers.getLast();
        while (curr != last) {
            Horse* horse = curr->getData();
            horse->unCheck();
            if (horse->getID() == followerID) {
                follower = horse;
            }
            if (horse->getID() == leaderID) {
                leader = horse;
            }
            curr = curr->getNext();
        }

        if ((follower == nullptr)||(leader == nullptr)){
            return false;
        }
        follower->inCircularReferance(this->totalMembers);
        return leader->alreadyChecked();
    }

    bool can_run_together(){
        int totalIndependant = 0;
        Node<Horse>* curr = this->herdMembers.getHead();
        Node<Horse>* last = this->herdMembers.getLast();
        while (curr != last) {
            Horse* horse = curr->getData();
            horse->unCheck();
            if (horse->independant()) {
                totalIndependant++;
            }
            curr = curr->getNext();
        }
        if (totalIndependant != 1){
            return false;
        }
        curr = this->herdMembers.getHead();
        last = this->herdMembers.getLast();
        while (curr != last) {
            Horse* horse = curr->getData();
            if (horse->inCircularReferance(this->totalMembers)){
                return false;
            }


            curr = curr->getNext();
        }


        return true;
    }
};

/**
    Node<Horse>* curr = this->herdMembers.getHead();
        Node<Horse>* last = this->herdMembers.getLast();
        while (curr != last) {
            Horse* horse = curr->getData();



            curr = curr->getNext();
    }
 */
