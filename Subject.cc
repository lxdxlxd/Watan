#include "Subject.h"
#include "Observer.h"

Subject::~Subject() {}

void Subject::attach(weak_ptr<Observer> o){
    ob.emplace_back(o);
}

void Subject::swap(int pos,weak_ptr<Observer> o){
    ob[pos]=o;
}

void Subject::notify_observers(Type t, int pos){
    if(t==Type::Trade||t==Type::Steal||t==Type::Check)
        ob[pos].lock()->being_notified(t,pos, *this);
    else{
        if(ob[3].lock())
            ob[3].lock()->being_notified(t,pos, *this);
    }
}

