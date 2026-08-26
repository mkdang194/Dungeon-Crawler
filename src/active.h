#ifndef ACTIVE_H
#define ACTIVE_H
#include <vector>
#include "passive.h"

// p4
class Active
{
    std::vector<Passive*> observers;
public:
    Active();
    void attach(Passive* p);
    void detach(Passive* p);
    void activate();

};

#endif // ACTIVE_H
