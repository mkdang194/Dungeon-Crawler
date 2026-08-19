#include "active.h"
#include <algorithm>

Active::Active() {}

void Active::attach(Passive *p)
{
    observers.push_back(p);
}

void Active::detach(Passive *p)
{
    observers.erase(std::remove(observers.begin(), observers.end(), p), observers.end());
}

void Active::activate()
{
    for (auto o : observers){
        o->notify();
    }
}
