#ifndef Observer_h
#define Observer_h
#include "Subscription.h"


class Subject;

class Observer {
public:
    virtual void being_notified(Type, int, Subject&)=0;
};

#endif /* Observer_h */

