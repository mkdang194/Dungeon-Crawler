#include "levelportal.h"

int LevelPortal::getTargetLevel() const
{
    return targetLevel;
}

void LevelPortal::setTargetLevel(int level)
{
    targetLevel = level;
}

int LevelPortal::getTargetPortal() const
{
    return targetPortal;
}

void LevelPortal::setTargetPortal(int portal)
{
    targetPortal = portal;
}