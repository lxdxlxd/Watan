#ifndef Subject_h
#define Subject_h
#include <vector>
#include <memory>
#include "Subscription.h"
using namespace std;

class Info;
class Observer;

class Subject {
    std::vector<weak_ptr<Observer>> ob;
public:
    virtual ~Subject()=0;
    virtual Info getInfo()=0;
    void attach(weak_ptr<Observer>);
    void swap(int,weak_ptr<Observer>);
    void notify_observers(Type,int);
};

#endif /* Subject_h */

