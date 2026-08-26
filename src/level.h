#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <memory>
#include <string>
#include "tile.h"
#include "character.h"
#include "portal.h"
#include "door.h"
#include "switch.h"
#include "levelportal.h"

class Level{

private:
    int rows;
    int cols;

    std::vector<std::vector<std::unique_ptr<Tile>>> grid;
    std::vector<std::unique_ptr<Character>> characters;
    std::vector<Portal*> portals;
    std::vector<Door*> doors;
    std::vector<Switch*> switches;
    std::vector<LevelPortal*> levelPortals;

public:
    Level(int r, int c) : rows(r), cols(c), grid(r){}
    int getRows() const;
    int getCols() const;
    Tile* getTile(int row, int col) const;
    void placeCharacter(std::unique_ptr<Character> c, int row, int col);
    void generate(const std::string& layout);
    Portal* findPortal();
    void loadFromFile(const std::string& filename);
    void linkPortal(int a, int b);
    void linkSwitchDoor(int sw, int door);
    std::vector<std::unique_ptr<Character>>& getCharacters();
    void removeCharacter(Character* character);
    void removeDeadCharacters();
    LevelPortal* getLevelPortal(int index);
};

#endif // LEVEL_H

