#include "pit.h"
#include "Character.h"
#include "ramp.h"
#include <stdexcept>

bool Pit::onLeave(Tile *destTile, Character *who)
{
    return dynamic_cast<Pit*>(destTile) || dynamic_cast<Ramp*>(destTile);
}

Tile* Pit::onEnter(Character* who)
{
    Tile* current = who->getCurrentTile();
    if(dynamic_cast<Ramp*>(current) || dynamic_cast<Pit*>(current))
    {
        return this;
    }

    return nullptr;
}