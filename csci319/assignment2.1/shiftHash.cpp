//
// Created by tom on 5/7/17.
//

#include "shiftHash.hpp"

unsigned int shiftHash (std::string in){
    unsigned int key = 0;
    for(int i = 0; i < in.length(); i++){
        key = ((key << 5)+key)^in[i];
    }

    return key;
}
