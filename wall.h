#ifndef WALL_H
#define WALL_H
#include "Tile.h"

class Wall : public Tile{
public:
    Wall(int r, int c) : Tile("#", r, c) {}
    Tile* onEnter(Character* who) override;
};

#endif // WALL_H
