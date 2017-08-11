/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/
#pragma once
#include "matrix.hpp"
#include "greedy.hpp"
#include <stack>
class BranchBound{
private:
    Matrix matrix;
    std::vector<Option>finalroute;
    std::vector<std::string>citynames;
public:
    BranchBound();
    ~BranchBound();

    void initialise(char[],char[]);
    void analyse(float);
    void results();
};
