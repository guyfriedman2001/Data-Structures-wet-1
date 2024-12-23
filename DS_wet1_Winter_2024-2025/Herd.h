#pragma once
#include "ProjectFiles.h"

class Herd { //: public IndexAble{
private:
    int herdId;
    LinkedList<Horse> herdMembers;
    int totalMembers;
public:
    Herd(int id):herdId(id), herdMembers(nullptr), totalMembers(0){}
    ~Herd() = default;
    int getID() const override{
        return this->herdId;
    }
    void setMembers(Horse* rep);
// comparison operators overload for use in tree methods.
    bool operator==(const Herd& otherHerd) const;
    bool operator>(const Herd& otherHerd) const;
    bool operator<(const Herd& otherHerd) const;

    bool add_horse(Horse* horse){
        for (Horse* horses : herdMembers){
            if (horse == horses){
                return false;
            }
        }
        herdMembers.add(horse);
        ++(this->totalMembers);
        return true;
    }
    
    bool leads(int followerID, int leaderID){
        Horse* follower = nullptr;
        Horse* leader = nullptr;
        for (Horse* horse : this->herdMembers){
            horse->unCheck();
            if (horse->getID() == followerID){
                follower = horse;
            }
            if (horse->getID() == leaderID){
                leader = horse;
            }
        }
        if ((follower == nullptr)||(leader == nullptr)){
            return false;
        }
        follower->inCircularReferance(this->totalMembers);
        return leader->alreadyChecked();
    }

    bool can_run_together(){
        int totalIndependant = 0;
        for (Horse* horse : herdMembers){
            horse->unCheck();
            if (horse->independant()){
                totalIndependant+=1;
            }
        }
        if (totalIndependant != 1){
            return false;
        }
        for (Horse* horse : herdMembers){
            if (horse->inCircularReferance(this->totalMembers)){
                return false;
            }
        }
        return true;
    }
};

