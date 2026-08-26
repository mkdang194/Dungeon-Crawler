#ifndef DOOR_H
#define DOOR_H
#include "tile.h"
#include "passive.h"

class Door : public Tile, public Passive
{
protected:
    bool open = false;
public:
    Door(int r, int c) : Tile ("T", r, c) {}
    virtual void notify() override;
    bool isOpen() const;
    virtual std::string getTexture() const override;
    Tile* onEnter(Character* who) override;
};

#endif // DOOR_H
