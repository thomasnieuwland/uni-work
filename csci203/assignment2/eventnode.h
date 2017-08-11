#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED
#include "event.h"

class EventNode{ // Node of EventQueue
private:
    EventNode* next;
    Event data;
public:
    EventNode();
    void createNode(Event);
    void setData(Event);
    void setNext(EventNode*);
    EventNode* getNext();
    Event getData();
};

EventNode::EventNode(){
    next = NULL;
}

void EventNode::createNode(Event in){
    data = in;
}

EventNode* EventNode::getNext(){
    return next;
}

Event EventNode::getData(){
    return data;
}

void EventNode::setData(Event a){
	data = a;
}

void EventNode::setNext(EventNode* a){
	next = a;
}

#endif // EVENTNODE_H_INCLUDED
