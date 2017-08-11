#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

class Event{
private:
    bool arrivalEvent; //If true, is arrival if false is finish event
    float e_time; //When the event occurs (arrival or finish)
    float duration; //Only used when arrival event to store duration;
public:
    Event();

    bool isArrival();
    float gete_time();
    float getduration();
    void createEvent(float,float,bool);

    friend bool operator<(Event&,Event&);
    friend bool operator>(Event&,Event&);
};

Event::Event(){
    arrivalEvent = false;
    e_time = -1;
    duration = -1;
}

bool Event::isArrival(){
    return arrivalEvent;
}

float Event::gete_time(){
    return e_time;
}

float Event::getduration(){
    return duration;
}

void Event::createEvent(float time, float dur, bool evntype){
    e_time = time;
    duration = dur;
    arrivalEvent = evntype;
}

bool operator<(Event& a,Event& b){
    if (a.gete_time() < b.gete_time()){
        return true;
    }

    else {
        return false;
    }
}

bool operator>(Event& a,Event& b){
    if (a.gete_time() > b.gete_time()){
        return true;
    }

    else {
        return false;
    }
}


#endif // EVENT_H_INCLUDED
