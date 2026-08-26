#include "portal.h"

Tile* Portal::onEnter(Character* who){
    if (!linkedPortal) return this;
    return linkedPortal;
}

void Portal::setTarget(Portal* p){
    linkedPortal = p;
}

void Portal::setTargetLevel(int level)
{
    targetLevel = level;
}

int Portal::getTargetLevel() const
{
    return targetLevel;
}

Portal* Portal::getTarget() const
{
    return linkedPortal;
}