#ifndef SWITCH_H
#define SWITCH_H
#include "active.h"
#include "Tile.h"

// p4
class Switch : public Tile, public Active
{
public:
    Switch(int r, int c) : Tile("?", r, c) {}
    Tile* onEnter(Character* who) override;
};

#endif // SWITCH_H
