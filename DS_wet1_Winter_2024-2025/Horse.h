#include "AllInclusions.h"

class Horse {
private:
    int horseId;
    double speed;
    int following;
    int herd;
public:
    Horse(int id, double speed);
    ~Horse() = default;
    int getID() const;
    double getSpeed() const;
    void setFollow(int horseToFollow);
    void setHerd(int herdId);
// comparison operators overload for use in tree methods.
    bool operator==(const Horse& otherHorse) const;
    bool operator>(const Horse& otherHorse) const;
    bool operator<(const Horse& otherHorse) const;
};