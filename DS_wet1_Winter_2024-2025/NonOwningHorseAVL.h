#pragma once
#include "AbstractAVL.h"
#include "/Users/guyfriedman/Documents/Technion/Semester_C_11.2024-02.2025/234218/HW1 wet/DS_wet1_Winter_2024-2025/wet1util.h"
class NonOwningHorseAVL : public AbstractAVL<Horse,StatusType>{
    virtual int getID() = 0;
};