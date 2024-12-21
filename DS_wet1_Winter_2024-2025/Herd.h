#pragma once
#include "ProjectFiles.h"
#include <cassert>

class Herd : public IndexAble{
private:
    int herdId;
    HorseMap* herdMembers;
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
    StatusType add_horse(int horseId, int speed){
        Horse newHorse(horseId,speed);
        this->herdMembers = this->herdMembers->insert(newHorse);
        StatusType temp = this->herdMembers->
    }
};

