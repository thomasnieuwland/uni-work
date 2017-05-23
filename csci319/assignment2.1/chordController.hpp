//
// Created by tom on 5/7/17.
//

#ifndef ASSIGNMENT2_1_CHORDCONTROLLER_HPP
#define ASSIGNMENT2_1_CHORDCONTROLLER_HPP


#include "chordNode.hpp"
#include "shiftHash.hpp"
#include <fstream>

class chordController {
private:
    chordNode* controller;
    int n;

    //Class only functions
    void updateTables();
    void deleteChord();

    //DEBUG FUNCTIONS;
    void powerCheck(int);
    int hashCheck(std::string);
    void printDataAll();
    void printTables();
    void printRing();

public:
    chordController();
    ~chordController();

    void initChord(int);
    void addPeer(int);
    void removePeer(int);
    void findKey(int);
    void insertData(std::string);
    void deleteString(std::string);
    void print(int);
    void read(char*);

};


#endif //ASSIGNMENT2_1_CHORDCONTROLLER_HPP
