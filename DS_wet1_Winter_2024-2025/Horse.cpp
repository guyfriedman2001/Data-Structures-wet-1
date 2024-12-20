#include "Horse.h"
#define NULL_ID -1

Horse::Horse(int id, double speed): horseId(id), speed(speed), following(NULL_ID),
herd(NULL_ID) {}

int Horse::getID() const{
    return this->horseId;
}

double Horse::getSpeed() const{
    return this->speed;
}
void Horse::setFollow(int horseToFollow){
    this->following = horseToFollow;
}
void Horse::setHerd(int herdId){
    this->herd = herdId;
}

bool Horse::operator==(const Horse& otherHorse) const{
    return this->horseId == otherHorse.horseId;
}
bool Horse::operator>(const Horse& otherHorse) const{
    return this->horseId > otherHorse.horseId;
}
bool Horse::operator<(const Horse& otherHorse) const{
    return this->horseId < otherHorse.horseId;
}

