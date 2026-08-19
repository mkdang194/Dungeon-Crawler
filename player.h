#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
public:
    Player(int strength, int stamina) : Character("@", strength, stamina){}
    bool isPlayer() const override;
};

#endif // PLAYER_H