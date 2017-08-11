#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED
#include "eventheap.h"
#include "eventqueue.h"
#include <fstream>

class Simulation{
private:
    std::ifstream input;
    EventHeap heap;
    EventQueue queue;
    float time;

public:
    Simulation();
    void initialise(char*);
    void run();
    void print();
};

Simulation::Simulation(){
    time = 0;
}
void Simulation::initialise(char* infile){
    int size;
    input.open(infile);
    if (!input.good()){
        throw 1;
    }
    else {
        input >> size;
        std::cout << std::setprecision(10);
        heap.makeHeap(size);
        Event firstEvent;
        float start, duration;
        firstEvent.createEvent(start,duration,true);
        heap.insert(firstEvent);
    }
}

void Simulation::run(){
    while (!heap.isEmpty() || !queue.isEmpty()){
        Event current, newEvent;
        current = heap.popRoot();
        print();
        time = current.gete_time();

        if(current.isArrival()){//Popped root is an arrival event
            if(!heap.isFull()){//Checks to see if servers are free immediately
                //Current arrival is converted to a finish event
                Event finishEvent;
                finishEvent.createEvent(time + current.getduration(), 0, false);
                heap.insert(finishEvent);

            }

            else{ //No servers available, arrival is placed in queue
                queue.enqueue(current);
            }

            if (input.good()){ //Checks to see if there are more arrivals to come in
                float start, duration;
                Event newArrival;
                input >> start >> duration;
                newArrival.createEvent(start,duration,true);
                heap.insert(newArrival);
            }
        }

        else{//Finish event popped
            if(!queue.isEmpty()){
                Event newArrival;
                newArrival = queue.dequeue();
                newEvent.createEvent(time + newArrival.getduration(), 0, false);
                heap.insert(newEvent);
            }
        }
    }
}

void Simulation::print(){
    std::cout << "*******************************************" << std::endl;
    std::cout << "Current time: " << time << std::endl << std::endl;
    std::cout << "Events" << std::endl;
    heap.print();
    std::cout << std::endl << "Queue: " << queue.getSize() << std::endl;
}

#endif // SIMULATION_H_INCLUDED
