#ifndef PORTAL_H
#define PORTAL_H
#include "Tile.h"

class Portal : public Tile{
protected:
    Portal* linkedPortal = nullptr;
    int targetLevel = 0;
public:
    Portal(int r, int c) : Tile("O", r, c) {}
    void setTarget(Portal* p);
    Tile* onEnter(Character* who) override;
    void setTargetLevel(int level);
    int getTargetLevel() const;
    Portal* getTarget() const;
};

#endif // PORTAL_H
