//
// Created by tom on 5/7/17.
//

#ifndef ASSIGNMENT2_1_DATAOBJ_HPP
#define ASSIGNMENT2_1_DATAOBJ_HPP

#include<string>

class dataObj {
private:
    int hash;
    std::string data;
public:
    int getHash();
    std::string getData();

    void setHash(int);
    void setData(std::string);
};


#endif //ASSIGNMENT2_1_DATAOBJ_HPP
