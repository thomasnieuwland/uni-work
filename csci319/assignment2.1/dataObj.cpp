//
// Created by tom on 5/7/17.
//

#include "dataObj.hpp"
int dataObj::getHash() {
    return hash;
}

std::string dataObj::getData() {
    return data;
}

void dataObj::setData(std::string input) {
    data = input;
}

void dataObj::setHash(int input) {
    hash = input;
}