#ifndef EVENTQUEUE_H_INCLUDED
#define EVENTQUEUE_H_INCLUDED
#include "eventnode.h"

class EventQueue{
private:
	EventNode* head;
	EventNode* tail;
	int size;
public:
	EventQueue();
	~EventQueue();

	void enqueue(Event);
	Event dequeue();
	bool isEmpty();
	int getSize();
};

EventQueue::EventQueue(){
	size= 0;
}

EventQueue::~EventQueue(){
    while (!isEmpty()){
        dequeue();
    }
}

int EventQueue::getSize(){
    return size;
}

bool EventQueue::isEmpty(){
    if (size == 0){
        return true;
    }

    else{
        return false;
    }
}

void EventQueue::enqueue(Event a){
	if(size == 0){
        EventNode* temp;
        temp =  new EventNode;
        temp->createNode(a);
        head = temp;
        tail = temp;
        size++;
	}

	else{
        EventNode* temp;
        temp = new EventNode;
        temp->createNode(a);
        tail->setNext(temp);
        tail = temp;
        size++;
	}
}

Event EventQueue::dequeue(){
    if (size > 0){
        Event tempEvent;
        EventNode* tempPointer;
        tempEvent = head->getData();
        tempPointer = head->getNext();
        delete head;
        head = tempPointer;
        size--;

        return tempEvent;
    }
}



#endif // EVENTQUEUE_H_INCLUDED
