#pragma once
#include "ProjectFiles.h"
#include <cassert>


//this tree has ownership of the horses.


class HorseTree : public HorseMap{
public:
    ~HorseTree() override{
        delete left;
        delete right;
        delete value;
    }
    void deleteValue(){
        delete value;
    }
};