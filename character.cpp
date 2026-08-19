#include "character.h"

std::string Character::getTexture() const{
    return texture;
}
Tile* Character::getCurrentTile() const{
    return currentTile;
}

void Character::setCurrentTile(Tile* t){
    currentTile = t;
}

int Character::getMaxHP()
{
    return 20 + stamina*5;
}

int Character::getStrength() const
{
    return strength;
}

int Character::getStamina() const
{
    return stamina;
}

int Character::getHitpoints() const
{
    return hitpoints;
}

void Character::takeDamage(int damage)
{
    hitpoints -= damage;
    if(hitpoints < 0)
        hitpoints = 0;
}

bool Character::isDead() const
{
    return hitpoints <= 0;
}

void Character::attack(Character* enemy)
{
    enemy->takeDamage(strength);
}
