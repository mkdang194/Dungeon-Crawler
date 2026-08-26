#ifndef PASSIVE_H
#define PASSIVE_H

// p4
class Passive
{
public:
    Passive();
    virtual void notify() = 0;
    virtual ~Passive() = default;
};

#endif // PASSIVE_H
