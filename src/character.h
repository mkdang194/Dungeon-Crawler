#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>

class Tile;

class Character{
protected:
    std::string texture;
    Tile* currentTile = nullptr;
    int strength;
    int stamina;
    int hitpoints;
public:
    Character(std::string t, int strength, int stamina) :
        texture(t), strength(strength), stamina(stamina)
    {
        hitpoints = getMaxHP();
    }

    std::string getTexture() const;
    Tile* getCurrentTile() const;
    void setCurrentTile(Tile* t);
    int getMaxHP();
    int getStrength() const;
    int getStamina() const;
    int getHitpoints() const;
    virtual bool isPlayer() const = 0;
    void takeDamage(int damage);
    void attack(Character* enemy);
    bool isDead() const;
};


#endif // CHARACTER_H
