#ifndef LOOTCHEST_H
#define LOOTCHEST_H

#include "tile.h"

class LootChest : public Tile
{
public:
    LootChest(int row, int col) : Tile("L", row, col){}
    Tile* onEnter(Character* who) override;
};

#endif // LOOTCHEST_H
