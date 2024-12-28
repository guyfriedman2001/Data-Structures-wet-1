#pragma once
#include "ProjectFiles.h"
#include "HorseList.h"
template<typename K, typename V>
class Pair{
public:
    K answer;
    V node;
    Pair(K answer, V nod){
        this->answer = answer;
        this->node = nod;
    }
    explicit Pair(V nod){
        this->answer = nullptr;
        this->node = nod;
    }
    Pair() {
        this->answer = nullptr;
        this->node = nullptr;
    }
    ~Pair(){
        this->node = nullptr;
    }
    void setAnswer(K answer) {
        this->answer = answer;
    }
    void setNode(V node) {
        this->node = node;
    }

};