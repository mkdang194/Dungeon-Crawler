#ifndef PIT_H
#define PIT_H

#include "tile.h"

// p4
class Pit : public Tile
{
public:
    Pit(int r, int c) : Tile("_", r, c) {}
    virtual bool onLeave(Tile* destTile, Character* who) override;
    Tile* onEnter(Character* who) override;
};

#endif // PIT_H
