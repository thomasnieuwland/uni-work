//
// Created by tom on 5/7/17.
//


#include "chordNode.hpp"
chordNode::chordNode(int identification,int chordn) {
    fingerTable = new chordNode*[chordn];
    id = identification;
    n = chordn;
}

chordNode::~chordNode() {
    delete[] fingerTable;
}

chordNode* chordNode::getSuccessor() {
    return fingerTable[0];
}

void chordNode::addPeer(chordNode *peer) {
    if (peer->id == id || peer->id == getSuccessor()->id){
        //Do nothing, node already exists
        std::cout << "Peer with ID " << peer->id << " already exists." << std::endl;
        delete peer; // clears memory
    }
    else if (peer->id > getSuccessor()->id){ //Greater than the successor
       if(id >= getSuccessor()->id) { //Successor is smallest node
           std::cout << id << ">" << peer->id << std::endl;
           peer->previous = this;
           getSuccessor()->previous = peer;
           peer->fingerTable[0] = getSuccessor();
           fingerTable[0] = peer;
           std::cout << "Peer " << peer->id << " added." << std::endl;

       }

       else{
           std::vector<int> ids;
           for (int i = 0; i < n ; ++i) {
               ids.push_back(fingerTable[i]->id);
           }

           std::sort(ids.begin(),ids.end());

           int findID;
           while(!ids.empty()){
               if (peer->id > ids.back()){
                   findID = ids.back();
                   break;
               }

               else{
                   ids.pop_back();
               }
           }

           for (int i = 0; i < n ; ++i) {
               if(fingerTable[i]->id == findID){
                   std::cout << id << ">";
                   fingerTable[i]->addPeer(peer);
                   break;
               }
           }
       }
    }
    else if (peer->id < getSuccessor()->id){ //Peer belongs between this node and the next node.
        std::cout << id << ">" << peer->id << std::endl;
        peer->previous = this;
        getSuccessor()->previous = peer;
        peer->fingerTable[0] = getSuccessor();
        fingerTable[0] = peer;
        std::cout << "Peer " << peer->id << " added." << std::endl;
    }

    else{
        std::cerr << "Error when adding peer";
    }



}

void chordNode::updateTable() {

    for (int i = 1; i < n; i++) {
        chordNode *current = getSuccessor();
        bool found = false;
        int findID = (id + (int) pow(2, i));

        if (findID > (int) pow(2, n)) {
            findID = findID % (int) pow(2, n);
            while (!found) {

                if (findID <= current->id) {
                    fingerTable[i] = current;
                    found = true;
                }

                else if (findID > current->getSuccessor()->id && current->id > current->getSuccessor()->id) {

                    fingerTable[i] = current->getSuccessor();
                    found = true;

                }

                else {
                    current = current->getSuccessor();
                }
            }
        }

        else {
            while (current->id > id) {
                current = current->getSuccessor();
            }

            while (!found) {

                if (findID <= current->id) {
                    fingerTable[i] = current;
                    found = true;
                } else if (findID > current->getSuccessor()->id && current->id > current->getSuccessor()->id) {
                    fingerTable[i] = current->getSuccessor();
                    found = true;

                } else {
                    current = current->getSuccessor();
                }
            }
        }
    }

    if(!storage.empty()){
        std::map<int,std::string>::iterator itr = storage.begin();
        while(itr != storage.end()) {
            if (previous->id >= itr->first) {
                previous->storage[itr->first] = itr->second;
                itr++;
            }

            else {
                break;
            }
        }

        if(itr != storage.begin()){
            storage.erase(storage.begin(),itr);
        }
    }

}

void chordNode::printTable() {
    for (int i = 0; i < n; ++i) {
        std::cout << " " << fingerTable[i]->id;
    }

    std::cout << std::endl;
}

void chordNode::addData(dataObj *data) {
    if (data->getHash() <= id){
        //Correct Node found
        if(storage.find(data->getHash()) == storage.end()){
            std::cout << id << std::endl;
            storage[data->getHash()] = data->getData();
            delete data;
        }

        else{
            std::cout << id << std::endl;
            std::cout << "Hash Collision detected. String shares the hash of an existing string. Discarding new string..." << std::endl;
            delete data;
        }
    }

    else{
        if (id > getSuccessor()->id){
            //Edge case where hash is bigger than largest node;
            std::cout << id << ">" << getSuccessor()->id << std::endl;
            getSuccessor()->storage[data->getHash()] = data->getData();
            delete data;
        }

        else{
            //Data is forwarded to the largest node that is smaller than the hash AND greater than the current ID
            int findID = -1;
            for (int i = 0; i < n ; ++i) {
                if (data->getHash() > fingerTable[i]->id && fingerTable[i]->id > findID && fingerTable[i]->id > id){
                    findID = fingerTable[i]->id;
                }
            }

            if(findID == -1){
                //No appropriate node was found, data most likely belongs to successor
                getSuccessor()->addData(data);
            }

            else{
                //Query is routed to the appropriate node;
                for (int i = 0; i < n ; ++i) {
                    if(fingerTable[i]->id == findID){
                        std::cout << id << ">";
                        fingerTable[i]->addData(data);
                        break;
                    }
                }
            }
        }
    }
}

void chordNode::printData(){
    for(std::map<int,std::string>::iterator itr=storage.begin();itr != storage.end();itr++){
        std::cout << itr->second << " (hash: " << itr->first << ")"<< std::endl;
    }
}

void chordNode::removeNode(int removeID) {
    if(removeID == id){
        //Move data to next node
        for(std::map<int,std::string>::iterator itr = storage.begin(); itr != storage.end(); itr++){
            getSuccessor()->storage[itr->first] = itr->second;
        }

        previous->fingerTable[0] = getSuccessor();
        getSuccessor()->previous = previous;
        std::cout << id;
        std::cout << std::endl << "Removed Node "<< id << ". Data moved to " << getSuccessor()->id << std::endl;
        delete this;
    }

    else if (removeID > id && removeID < getSuccessor()->id){
        std::cout << "Node doesn't exist" << std::endl;
    }

    else if (removeID > id && id > getSuccessor()->id){
        std::cout << "Node doesn't exist" << std::endl;
    }

    else{
        //Finds next node smaller than or equal to ID searching for
        int next = -1;
        for (int i = 0; i < n ; ++i) {
            if(fingerTable[i]->id > next && fingerTable[i]->id <= removeID){
                next = fingerTable[i]->id;
            }
        }

        for (int j = 0; j < n ; ++j) {
            if(fingerTable[j]->id == next){
                std::cout << id << ">";
                fingerTable[j]->removeNode(removeID);
                break;
            }
        }
    }


}

void chordNode::findKey(int key) {
    if (key <= id){
        if(storage.find(key) != storage.end()){
            std::cout << id << std::endl;
            std::cout <<"Key " << key << " found at Node " << id << std::endl;

        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else if (id > getSuccessor()->id){ // Key larger than largest node
        if(getSuccessor()->storage.find(key) != getSuccessor()->storage.end()){
            std::cout << id << std::endl;
            std::cout <<"Key " << key << " found at Node " << getSuccessor()->id << std::endl;

        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else{
        int next = -1;
        for (int i = 0; i < n; ++i) {
            if(key <= fingerTable[i]->id && fingerTable[i]->id > next){
                next = fingerTable[i]->id;
            }
        }

        if (next == -1){
            getSuccessor()->findKey(key);
        }

        else{
            for (int i = 0; i < n; ++i) {
                if(fingerTable[i]->id == next){
                    std::cout << id << ">";
                    fingerTable[i]->findKey(key);
                    break;
                }
            }
        }
    }
}

void chordNode::deleteKey(int key) {
    if (key <= id){
        if(storage.find(key) != storage.end()){
            std::cout << id << std::endl;
            std::cout << "Deleted \"" << storage.find(key)->second << "\" (Key: " << key << ")" <<std::endl;
            storage.erase(key);
        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else if (id > getSuccessor()->id){ // Key larger than largest node
        if(getSuccessor()->storage.find(key) != getSuccessor()->storage.end()){
            std::cout << id << std::endl;
            std::cout << "Deleted \"" << getSuccessor()->storage.find(key)->second << "\" (Key: " << key << ")" <<std::endl;
            getSuccessor()->storage.erase(key);


        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else{
        int next = -1;
        for (int i = 0; i < n; ++i) {
            if(key <= fingerTable[i]->id && fingerTable[i]->id > next){
                next = fingerTable[i]->id;
            }
        }

        if (next == -1){
            getSuccessor()->deleteKey(key);
        }

        else{
            for (int i = 0; i < n; ++i) {
                if(fingerTable[i]->id == next){
                    std::cout << id << ">";
                    fingerTable[i]->deleteKey(key);
                    break;
                }
            }
        }
    }
}

void chordNode::print(int key) {
    if (key <= id){
        if(storage.find(key) != storage.end()){
            std::cout << id << std::endl << std::endl;
            std::cout <<"Key " << key << " found at Node " << id << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            std::cout << "Finger table for this node: ";
            printTable();
            std::cout << "Data stored at this Node: " << std::endl;
            printData();

        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else if (id > getSuccessor()->id){ // Key larger than largest node
        if(getSuccessor()->storage.find(key) != getSuccessor()->storage.end()){
            std::cout << id << std::endl;
            std::cout <<"Key " << key << " found at Node " << getSuccessor()->id << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            std::cout << "Finger table for this node: ";
            printTable();
            std::cout << "Data stored at this Node: ";
            printData();

        }

        else{
            std::cout << id << std::endl;
            std::cout << "Key does not exist." << std::endl;
        }
    }

    else{
        int next = -1;
        for (int i = 0; i < n; ++i) {
            if(key <= fingerTable[i]->id && fingerTable[i]->id > next){
                next = fingerTable[i]->id;
            }
        }

        if (next == -1){
            getSuccessor()->print(key);
        }

        else{
            for (int i = 0; i < n; ++i) {
                if(fingerTable[i]->id == next){
                    std::cout << id << ">";
                    fingerTable[i]->print(key);
                    break;
                }
            }
        }
    }
}