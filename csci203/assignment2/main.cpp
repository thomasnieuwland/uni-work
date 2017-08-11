#include <iostream>
#include "simulation.h"
using namespace std;

int main(int argc, char *argv[])
{
    Simulation sim;
    try{
        sim.initialise(argv[1]);
    }

    catch(int err){
        cout << "Could not load file, aborting program!" << endl;
        return 0;
    }

    sim.run();

    return 0;
}
