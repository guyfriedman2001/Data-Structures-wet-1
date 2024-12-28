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
    explicit Herd(int id) : herdId(id), herdMembers(), totalMembers(0) {}

    ~Herd() = default;

    int getID() const {
        return this->herdId;
    }

    // comparison operators overload for use in tree methods.
    bool operator==(const Herd &otherHerd) const {
        return this->getID() == otherHerd.getID();
    }

    bool operator>(const Herd &otherHerd) const {
        return this->getID() > otherHerd.getID();
    }

    bool operator<(const Herd &otherHerd) const {
        return this->getID() < otherHerd.getID();
    }

    bool add_horse(Horse* horse) {
        if(herdMembers.getHead() != nullptr && herdMembers.getTail() != nullptr) {
            if(herdMembers.getHead()->getData() != nullptr && herdMembers.getTail()->getData() != nullptr) {
                for (auto it = herdMembers.begin(); it != herdMembers.end(); ++it) {
                    if (*horse == *it) {
                        return false;
                    }
                }
            }
        }
        herdMembers.insert(horse);
        ++(this->totalMembers);
        horse->join_herd(this);
        return true;
    }

        bool leads(int followerID, int leaderID) {
            Horse *follower = nullptr;
            Horse *leader = nullptr;
            for (auto it = herdMembers.begin(); it != herdMembers.end(); ++it) {
                it.getCurrentData()->unCheck();
                if (it.getCurrentData()->getID() == followerID) {
                    follower = &(*it);
                }
                if (it.getCurrentData()->getID() == leaderID) {
                    leader = &(*it);
                }
            }
            if ((follower == nullptr) || (leader == nullptr)) {
                return false;
            }
            follower->inCircularReferance(this->totalMembers);
            return leader->alreadyChecked();
        }

        bool can_run_together() {
            int totalIndependant = 0;
            for (auto it = herdMembers.begin(); it != herdMembers.end(); ++it) {
                it.getCurrentData()->unCheck();
                if (it.getCurrentData()->independant()) {
                    totalIndependant += 1;
                }
            }
            if (totalIndependant != 1) {
                return false;
            }
            for (auto it = herdMembers.begin(); it != herdMembers.end(); ++it) {
                if (it.getCurrentData()->inCircularReferance(this->totalMembers)) {
                    return false;
                }
            }
            return true;
        }
    };