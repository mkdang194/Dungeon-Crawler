#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"

class Monster : public Character
{
public:
    Monster() : Character("M", 5, 5) {}

    bool isPlayer() const override;
};

#endif