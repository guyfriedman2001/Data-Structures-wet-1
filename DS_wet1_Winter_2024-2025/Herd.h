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
        for (Horse horses : herdMembers){
            if (*horse == horses){
                return false;
            }
        }
        herdMembers.insert(horse);
        ++(this->totalMembers);
        horse->join_herd(this);
        return true;
    }
    
    bool leads(int followerID, int leaderID){
        Horse* follower = nullptr;
        Horse* leader = nullptr;
        for (Horse horse : this->herdMembers){
            horse.unCheck();
            if (horse.getID() == followerID){
                follower = &horse;
            }
            if (horse.getID() == leaderID){
                leader = &horse;
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
        for (Horse horse : herdMembers){
            horse.unCheck();
            if (horse.independant()){
                totalIndependant+=1;
            }
        }
        if (totalIndependant != 1){
            return false;
        }
        for (Horse horse : herdMembers){
            if (horse.inCircularReferance(this->totalMembers)){
                return false;
            }
        }
        return true;
    }
};

