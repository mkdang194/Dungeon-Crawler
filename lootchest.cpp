#include "lootchest.h"
#include "character.h"


Tile* LootChest::onEnter(Character* who)
{
    if(who->isPlayer())
    {
        return this;
    }
    return this;
}