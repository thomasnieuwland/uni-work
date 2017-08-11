/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/
#include "matrix.hpp"
#include "greedy.hpp"
#include "branchbound.hpp"
using namespace std;

int main(int argc, char *argv[]){
    Greedy groads,gflights;
    BranchBound broads, bflights;
    float gUB, fUB;

    cout << "----Roads----" << endl;
    groads.initialise(argv[1],argv[3]);
    groads.analyse();
    gUB = groads.results();

    broads.initialise(argv[1],argv[3]);
    broads.analyse(gUB);
    broads.results();

    cout << endl << "----Flights----" << endl;
    gflights.initialise(argv[2],argv[3]);
    gflights.analyse();
    fUB = gflights.results();

    bflights.initialise(argv[2],argv[3]);
    bflights.analyse(fUB);
    bflights.results();

    return 0;
}
