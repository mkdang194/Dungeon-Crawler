#ifndef RAMP_H
#define RAMP_H

#include "tile.h"

// p4
class Ramp : public Tile
{
public:
    Ramp(int r, int c) : Tile("<", r, c) {}
    Tile* onEnter(Character* who) override;

};

#endif // RAMP_H
