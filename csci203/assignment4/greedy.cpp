/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/
#include "greedy.hpp"
using namespace std;

Option::Option(){
    start = -1;
    finish = -1;
    cost = -1;
}

Option::~Option(){
    //Intentionally left blank
}

bool operator> (const Option &a, const Option &b){
    return (a.cost > b.cost);
}

bool operator< (const Option &a, const Option &b){
    return (a.cost < b.cost);
}

Greedy::Greedy(){
    visited = NULL;
}

Greedy::~Greedy(){
    if (visited != NULL){
        delete[] visited;
    }
}

void Greedy::initialise(char matrixfile[],char cityfile[]){
    matrix.intialise(matrixfile);
    visited = new bool[matrix.vertices];
    for (int i = 0; i < matrix.vertices; i++){ //Sets visited to false
        visited[i] = false;
    }

    ifstream inf;
    inf.open(cityfile);
    if (!inf.good()){
        cout << "Cities file could not be opened" << endl;
    }

    else{
        string tempstring;
        while(getline(inf,tempstring)){
            citynames.push_back(tempstring);
        }
    }

}

bool Greedy::finished(){
    bool retval = true;
    for(int i = 0; i < matrix.vertices; i++){
        if(visited[i] == false){
            retval = false;
            break;
        }
    }

    return retval;
}

void Greedy::analyse(){
    for (int i = 0; i < matrix.vertices; i++){
        if(matrix.matrix[0][i] != 0){
            Option tOp; //Temp Option
            tOp.start = 0;
            tOp.finish = i;
            tOp.cost = matrix.matrix[0][i];
            options.push_back(tOp);
        }
    }


    sort(options.begin(),options.end());
    vector<Option>::iterator itr = options.begin();

    int newStart = itr->finish;
    visited[0] = true;
    visited[itr->finish] = true;
    finalroute.push_back(*itr);

    while (!finished()){
        options.clear();
        for (int i = 0; i < matrix.vertices; i++){
            if(matrix.matrix[newStart][i] != 0){
                Option tOp; //Temp Option
                tOp.start = newStart;
                tOp.finish = i;
                tOp.cost = matrix.matrix[newStart][i];
                options.push_back(tOp);
            }
        }

        sort(options.begin(),options.end());
        itr = options.begin();

        for(itr = options.begin(); itr != options.end(); itr++){
            if(visited[itr->finish] == false){
                newStart = itr->finish;

                visited[itr->finish] = true;
                finalroute.push_back(*itr);
                break;
            }
        }
    }

    Option tOp; //Temp Option
    tOp.start = itr->finish;
    tOp.finish = 0;
    tOp.cost = matrix.matrix[itr->finish][0];
    finalroute.push_back(tOp);

}

float Greedy::results(){
    float totalcost = 0;
    cout << endl << "1. Greedy algorithm: " << endl;
    cout << "Number of Cities: " << matrix.vertices << endl;
    cout << "Tour: " << endl << "\t";

    vector<Option>::iterator itr = finalroute.begin();
    cout << itr->start;

    for(itr = finalroute.begin(); itr != finalroute.end(); itr++){
        cout << " " << itr->finish;
        totalcost+= itr->cost;
    }

    itr = finalroute.begin();
    cout << endl << '\t' << citynames[itr->start] << endl;
    cout.clear();

    for(itr = finalroute.begin(); itr != finalroute.end(); itr++){
        cout << '\t' << citynames[itr->finish] << endl;
    }

    cout << "Total Cost: " << totalcost << endl;

    return totalcost;
}
