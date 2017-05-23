//
// Created by tom on 5/7/17.
//

#ifndef ASSIGNMENT2_1_CHORDNODE_HPP
#define ASSIGNMENT2_1_CHORDNODE_HPP
#include<vector>
#include <iostream>
#include <cmath>
#include<algorithm>
#include<map>
#include "dataObj.hpp"

class chordNode {
public:
    int id;
    int n;
    chordNode* previous;
    chordNode** fingerTable;
    std::map<int,std::string> storage;

    chordNode(int,int);
    ~chordNode();

    chordNode* getSuccessor();
    void addPeer(chordNode*);
    void addData(dataObj*);
    void updateTable();
    void printTable();
    void removeNode(int);
    void printData();
    void findKey(int);
    void deleteKey(int);
    void print(int);
};


#endif //ASSIGNMENT2_1_CHORDNODE_HPP
