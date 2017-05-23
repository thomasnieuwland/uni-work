//
// Created by tom on 5/7/17.
//


#include "chordController.hpp"


chordController::chordController() {
    controller = NULL;
    n = 0;
}

chordController::~chordController() {
    deleteChord();
}

void chordController::initChord(int in) {
    if(in <= 31 && in >= 2) {
        if (controller != NULL) {
            deleteChord();

            std::cout << "Old chord deleted." << std::endl;
        }

        controller = new chordNode(0, in);

        controller->fingerTable[0] = controller;
        n = in;
        std::cout << "Chord intialised" << std::endl;
        printRing();
    }

    else{
        std::cerr << "Invalid initialisation value" << std::endl;
    }

}

void chordController::deleteChord() {
    if(controller != NULL) {
        chordNode *toDelete;
        chordNode *current = controller->getSuccessor();

        while (current != controller) {
            toDelete = current;
            current = current->getSuccessor();
            delete toDelete;
        }

        delete controller;
    }
}

void chordController::addPeer(int id) {
    if(id >= 0 && id < (int)pow(2,n)) {
        chordNode *peer = new chordNode(id, n);
        std::cout << std::endl << "Adding Peer " << id << std::endl;
        std::cout << "Path taken: ";
        controller->addPeer(peer);
        updateTables();
        printRing();
    }

    else{
        std::cout << std::endl << "Node " << id << " outside expected range." << std::endl;
    }
}

void chordController::printRing() {
    std::cout << "Current Ring:" << std::endl;
    chordNode* current = controller->getSuccessor();
    std::cout << controller->id;
    while(current != controller){
        std::cout << ">" << current->id;
        current = current->getSuccessor();
    }

    std::cout << ">" <<controller->id << std::endl;
}

void chordController::updateTables() {
    chordNode* current = controller->getSuccessor();
    controller->updateTable();
    while(current != controller){
        current->updateTable();
        current = current->getSuccessor();
    }
}

void chordController::printTables() {
    chordNode* current = controller->getSuccessor();
    controller->printTable();
    while(current != controller){
        current->printTable();
        current = current->getSuccessor();
    }
}

void chordController::powerCheck(int id) {
    for (int i = 0; i < n; ++i) {
        std::cout << (id +(int)pow(2,i))%(int)pow(2,n) << " ";
    }
}

void chordController::insertData(std::string input){
    dataObj* data = new dataObj;
    data->setData(input);
    data->setHash(shiftHash(input)%(int)pow(2,n));
    std::cout << std::endl << "Adding string \""<< input << "\" (key: " << data->getHash() << ")" << std::endl;
    std::cout << "Path Taken: ";
    controller->addData(data);
}

int chordController::hashCheck(std::string in) {
    return shiftHash(in)%(int)pow(2,n);
}

void chordController::removePeer(int id) {
    std::cout << std::endl << "Deleting Node " << id << "..." << std::endl;
    std::cout << "Path taken: ";
    if(id == controller->id){
        //Controller node is being deleted. Prepares for new controller
        chordNode* newController = controller->getSuccessor();
        controller->removeNode(id);
        controller = newController;
    }

    else{
        controller->removeNode(id);
    }

    updateTables();
}

void chordController::printDataAll() {
    chordNode* current = controller->getSuccessor();
    controller->printData();
    while(current != controller){
        current->printData();
        current = current->getSuccessor();
    }
}

void chordController::findKey(int key) {
    std::cout<< std::endl << "Finding Key " << key << "..." << std::endl;
    std::cout << "Path taken: ";
    controller->findKey(key);
}

void chordController::deleteString(std::string input) {
    int key = shiftHash(input)%(int)pow(2,n);
    std::cout<< std::endl << "Deleting Key " << key << "..." << std::endl;
    std::cout << "Path taken: ";
    controller->deleteKey(key);
}

void chordController::print(int key) {
    std::cout << std::endl << "Printing information about the node where Key " << key << " is stored..." <<std::endl;
    std::cout << "Path taken: ";
    controller->print(key);
}

void chordController::read(char* filename) {
    std::ifstream infile;
    infile.open(filename);

    if(infile.is_open()) {
        while (infile.good()) {
            std::string temp, arg;
            std::getline(infile,temp,'');
            unsigned long hash = arg.find('#');
            if (hash == std::string::npos){
                hash = arg.length();
            }

            else{
                arg = arg.substr(0,hash);
            }
            

            if(temp == "initchord"){

                initChord(atoi(arg.c_str()));
            }

            else if(temp == "addpeer"){
                addPeer(atoi(arg.c_str()));
            }

            else if(temp == "removepeer"){
                removePeer(atoi(arg.c_str()));
            }

            else if(temp == "findkey"){
                findKey(atoi(arg.c_str()));
            }

            else if(temp == "print"){
                print(atoi(arg.c_str()));
            }

            else if(temp == "insert"){
                insertData(arg);
            }

            else if(temp == "delete"){
                deleteString(arg);
            }


        }
    }

    else{
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cout << "\t\t Could not find file!" << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    }

}
