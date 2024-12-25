#include "Herd.h"

//Herd::Herd(int id) : herdId(id), herdMembers(), totalMembers(0) {}

//int Herd::getID() const{return this->herdId;}
//void Herd::setMembers(Horse* rep) {this->herdMembers = rep;}

bool Herd::operator==(const Herd& otherHerd) const{
    return this->herdId == otherHerd.herdId;
}
bool Herd::operator>(const Herd& otherHerd) const{
    return this->herdId > otherHerd.herdId;
}
bool Herd::operator<(const Herd& otherHerd) const{
    return this->herdId < otherHerd.herdId;
}
