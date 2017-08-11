/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/
#ifndef GREEDY_HPP_INCLUDED
#define GREEDY_HPP_INCLUDED

#include "matrix.hpp"
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>

class Option{
public:
    int start;
    int finish;
    float cost;

    Option();
    ~Option();

    friend bool operator> (const Option &a, const Option &b);
    friend bool operator< (const Option &a, const Option &b);
};

class Greedy{
private:
    bool *visited;
    Matrix matrix;
    std::vector<Option> options;
    std::vector<Option> finalroute;
    std::vector<std::string> citynames;

public:
    Greedy();
    ~Greedy();

    void initialise(char[],char[]); //Initialises the class matrix and bool array
    bool finished();
    void analyse();
    float results();
};

#endif
