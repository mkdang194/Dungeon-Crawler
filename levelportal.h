#ifndef LEVELPORTAL_H
#define LEVELPORTAL_H

#include "Tile.h"

class LevelPortal : public Tile
{
private:
    int targetLevel;
    int targetPortal;

public:
    LevelPortal(int row,int col)
        : Tile("P",row,col), targetLevel(0), targetPortal(0){}

    int getTargetLevel() const;
    void setTargetLevel(int level);

    int getTargetPortal() const;
    void setTargetPortal(int portal);
};

#endif