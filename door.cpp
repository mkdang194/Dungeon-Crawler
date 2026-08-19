#include "door.h"
#include "character.h"
#include <iostream>

void Door::notify()
{
    open = !open;
    std::cout
        << "Door is "
        << (open ? "open" : "closed")
        << ".\n";
}

bool Door::isOpen() const
{
    return open;
}

std::string Door::getTexture() const
{
    if(character)
    {
        return character->getTexture();
    }

    return open ? "/" : "T";
}

Tile *Door::onEnter(Character *who)
{
    return open ? this : nullptr;
}
