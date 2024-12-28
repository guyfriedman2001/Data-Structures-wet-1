#include "ProjectFiles.h"
#include <cassert>

#define NULL_ID (-1)

//Horse::Horse(int id, double speed): horseId(id), speed(speed), following(NULL_ID), herd(NULL_ID), herdInsertions(0), follows(nullptr), special_bool(false) {}

//int Horse::getID() const{return this->horseId;}

//double Horse::getSpeed() const{return this->speed;}
//void Horse::setFollow(int horseToFollow){this->following = horseToFollow;}
//void Horse::setFollow(Horse* horseToFollow){assert(this->herd == horseToFollow->herd);}

//void Horse::setHerd(int herdId){this->herd = herdId;}

bool Horse::operator==(const Horse& otherHorse) const{
    return this->horseId == otherHorse.horseId;
}
bool Horse::operator>(const Horse& otherHorse) const{
    return this->horseId > otherHorse.horseId;
}
bool Horse::operator<(const Horse& otherHorse) const{
    return this->horseId < otherHorse.horseId;
}

void Horse::join_herd(Herd* herd){
    this->herdID = herd->getID();
    ++(this->herdInsertions);
}


/**
 *
bool Horse::independant(){
    if (this->follows == nullptr){ return true;}
    if (this->follows->herd != this->herd){return true;} //FIXME can a horse follow itself?
    if (true){return true;} //TODO add check that insertion id is correct
    return false;
}
 */

