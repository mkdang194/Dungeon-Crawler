#ifndef FLOOR_H
#define FLOOR_H
#include "Tile.h"

class Floor : public Tile{
public:
    Floor(int r, int c) : Tile(".", r, c) {}
    Tile* onEnter(Character* who) override;
};

#endif // FLOOR_H
