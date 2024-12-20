#include "AllInclusions.h"

class Herd {
private:
    int herdId;
    Horse* herdMembers;
public:
    Herd(int id);
    ~Herd() = default;
    int getID() const;
    void setMembers(Horse* rep);
// comparison operators overload for use in tree methods.
    bool operator==(const Herd& otherHerd) const;
    bool operator>(const Herd& otherHerd) const;
    bool operator<(const Herd& otherHerd) const;
};

