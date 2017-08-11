#ifndef EVENTHEAP_H_INCLUDED
#define EVENTHEAP_H_INCLUDED
#include "event.h"
#include <iostream>
#include <iomanip>

class EventHeap {
private:
    int maxsize;
    int currentsize;
    Event* heap;

public:
    EventHeap();
    ~EventHeap();
    void makeHeap(int);
    void siftUp(int);
    void siftDown(int);
    void insert(Event);
    Event popRoot();
    void print();
    bool isEmpty();
    bool isFull();
};

EventHeap::EventHeap(){
    heap = NULL;
    maxsize = 0;
    currentsize = 0;
}

Event EventHeap::popRoot(){
    if (currentsize > 0){
        Event temp;
        temp = heap[0];
        heap[0] = heap[currentsize-1];
        siftDown(0);
        currentsize--;
        return temp;
    }
}

EventHeap::~EventHeap(){
    while (currentsize != 0){
        popRoot();
    }

    delete[] heap;
}

bool EventHeap::isFull(){
    if (currentsize == maxsize-1){
        return true;
    }

    else{
        return false;
    }
}

bool EventHeap::isEmpty(){
	if (currentsize <= 0){
		return true;
	}

	return false;
}

void EventHeap::makeHeap(int size){
    maxsize = size + 1; //Number of servers + 1 place for current processing event
    heap = new Event [size];
}

void EventHeap::siftUp(int i){
    if (i == 0) {
        return;
    }

    else {
        int p = (i - 1)/2;

        if (heap[p] < heap[i]) {
            return;
        }

        else {
            Event temp = heap[p];

            heap[p] = heap[i];
            heap[i] = temp;
            siftUp(p);
        }
    }
}

void EventHeap::siftDown(int i){
    int c = (i * 2) + 1;
    if (c >= currentsize) {
        return; // makes sure we don't leave the bounds of the array
    }

    else if (c + 1 >= currentsize) { //Makes sure we don't leave the bounds of the array
		if (heap[i] > heap[c]) { // Makes sure size = 2 works
			Event temp;
			temp = heap[c];
			heap[c] = heap[i];
			heap[i] = temp;
			siftDown(c);
		}
        return;
    }

    if (heap[c] > heap[c + 1]) {
        c = c + 1;
    }

    if (heap[i] > heap[c]) {
        Event temp;
        temp = heap[c];
        heap[c] = heap[i];
        heap[i] = temp;
        siftDown(c);
    }
}

void EventHeap::insert(Event in){
    if (currentsize < maxsize){
        currentsize++;
        heap[currentsize-1] = in;
        siftUp(currentsize-1);
    }
}

void EventHeap::print(){
	EventHeap tempHeap;
	Event temp;

	tempHeap.makeHeap(maxsize-1);

	while(!isEmpty()){
		temp = popRoot();
		if (temp.isArrival()){
			std::cout << "Arrival: " << temp.gete_time() << " Duration: " << temp.getduration() << std::endl;
		}

		else{
			std::cout << "Finish: " << temp.gete_time() << std:: endl;
		}


		tempHeap.insert(temp);
	}

	while(!tempHeap.isEmpty()){
		insert(tempHeap.popRoot());
	}


}

#endif // EVENTHEAP_H_INCLUDED
