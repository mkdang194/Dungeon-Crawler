#include "Level.h"
#include "Character.h"
#include "Tile.h"
#include "Floor.h"
#include "Wall.h"
#include "Portal.h"
#include "door.h"
#include "lootchest.h"
#include "ramp.h"
#include "pit.h"
#include "switch.h"
#include "levelportal.h"
#include "monster.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

//ownership
//game world (owns everything: tiles, character,etc)

Tile* Level::getTile(int row, int col) const
{
    if(row < 0 || row >= rows || col < 0 || col >= cols) {
        return nullptr;
    }
    return grid[row][col].get(); // .get() return raw ptr Tile*
}

void Level::generate(const std::string& layout){
    std::cout << "GENERATE CALLED" << std::endl;
    std::vector<std::pair<int,int>> monsterSpawns;
    grid.clear();
    grid.resize(rows);

    characters.clear();
    if (layout.size() != rows * cols){
        throw std::runtime_error("Invalid layout size");
    }
    levelPortals.clear();
    portals.clear();
    doors.clear();
    switches.clear();
    int index = 0;
    for(int r = 0; r < rows; r++){
        std::vector<std::unique_ptr<Tile>> line;
        for(int c = 0; c < cols; c++) {
            char symbol = layout[index++]; // parse string
            switch (symbol){
            case '#':{
                line.push_back(std::make_unique<Wall>(r,c));
                break;
            }
            case '.':{
                line.push_back(std::make_unique<Floor>(r,c));
                break;
            }
            case 'O':{
                auto portal = std::make_unique<Portal>(r,c);
                portals.push_back(portal.get());
                line.push_back(std::move(portal));
                break;
            }
            // p4
            case '<':{
                line.push_back(std::make_unique<Ramp>(r,c));
                break;
            }
            case '_':{
                line.push_back(std::make_unique<Pit>(r,c));
                break;
            }
            case 'T':{
                auto door = std::make_unique<Door>(r,c);
                doors.push_back(door.get());
                line.push_back(std::move(door));
                break;
            }
            case '?':{
                auto sw = std::make_unique<Switch>(r,c);
                switches.push_back(sw.get());
                line.push_back(std::move(sw));
                break;
            }
            case 'P':
            {
                auto portal = std::make_unique<LevelPortal>(r, c);
                levelPortals.push_back(portal.get());
                line.push_back(std::move(portal));
                break;
            }
            case 'M':
            {
                monsterSpawns.emplace_back(r, c);
                line.push_back(std::make_unique<Floor>(r, c));
                break;
            }
            case 'L':
            {
                line.push_back(std::make_unique<LootChest>(r, c));
                break;
            }
            default: {throw std::runtime_error("Unknown tile symbol");}
            }
        }
        grid[r] = std::move(line);
    }
    for(const auto& pos : monsterSpawns)
    {
        auto monster = std::make_unique<Monster>();
        placeCharacter(std::move(monster), pos.first, pos.second);
    }
}

void Level::placeCharacter(std::unique_ptr<Character> c, int row, int col){
    Tile* tile = getTile(row, col); // spawn position
    if (!tile) { // if tile exists
        throw std::runtime_error("Invalid position!");
    }
    Tile* target = tile->onEnter(c.get()); // check if character can enter tile
    if (!target) { // if target is invalid
        throw std::runtime_error("Cannot place character here");
    }
    target->setCharacter(c.get()); // which character stand on tile
    c->setCurrentTile(target); // which tile character is standing on
    characters.push_back(std::move(c)); // transfer ownership to level
}

int Level::getRows() const{
    return rows;
}

int Level::getCols() const{
    return cols;
}

Portal* Level::findPortal()
{
    for (auto& row : grid)
    {
        for (auto& tile : row)
        {
            if (auto portal =
                dynamic_cast<Portal*>(tile.get()))
            {
                return portal;
            }
        }
    }
    return nullptr;
}

void Level::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file)
        throw std::runtime_error("Cannot open level file.");
    std::string layout;
    layout.reserve(rows * cols);
    std::string line;

    // Read map
    for(int i = 0; i < rows; i++)
    {
        std::getline(file, line);
        layout += line;
    }
    generate(layout);
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;
        if(line == "PORTALS")
        {
            while(std::getline(file, line))
            {
                if(line.empty())
                    break;
                std::stringstream ss(line);
                int a,b;
                ss >> a >> b;
                linkPortal(a,b);
            }
        }

        if(line == "SWITCHES")
        {
            while(std::getline(file, line))
            {
                if(line.empty())
                    break;
                std::stringstream ss(line);
                int sw,door;
                ss >> sw >> door;
                linkSwitchDoor(sw,door);
            }
        }
        if(line == "LEVELPORTALS")
        {
            while(std::getline(file,line))
            {
                if(line.empty())
                    break;
                std::stringstream ss(line);
                int portalIndex;
                int targetLevel;
                int targetPortal;
                ss >> portalIndex
                    >> targetLevel
                    >> targetPortal;
                if(portalIndex < 0 ||
                    portalIndex >= levelPortals.size())
                {
                    throw std::runtime_error(
                        "Invalid level portal index.");
                }
                levelPortals[portalIndex]->setTargetLevel(targetLevel);
                levelPortals[portalIndex]->setTargetPortal(targetPortal);
            }
        }
    }
}

void Level::linkPortal(int a, int b)
{
    if(a < 0 || b < 0 ||
        a >= portals.size() ||
        b >= portals.size())
    {
        throw std::runtime_error("Invalid portal index.");
    }
    portals[a]->setTarget(portals[b]);
    portals[b]->setTarget(portals[a]);
}

void Level::linkSwitchDoor(int sw, int door)
{
    if(sw < 0 || door < 0 ||
        sw >= switches.size() ||
        door >= doors.size())
    {
        throw std::runtime_error("Invalid switch/door index.");
    }
    switches[sw]->attach(doors[door]);
}

std::vector<std::unique_ptr<Character>>& Level::getCharacters()
{
    return characters;
}

void Level::removeCharacter(Character* character)
{
    character->getCurrentTile()->setCharacter(nullptr);
    character->setCurrentTile(nullptr);
    auto it = std::remove_if(characters.begin(), characters.end(),
                             [character](const std::unique_ptr<Character>& c)
                             {return c.get() == character;});
    characters.erase(it, characters.end());
}

void Level::removeDeadCharacters()
{
    for(auto& c : characters)
    {
        if(c->isDead())
        {
            Tile* tile = c->getCurrentTile();
            if(tile && tile->getCharacter() == c.get())
            {
                tile->setCharacter(nullptr);
            }
            c->setCurrentTile(nullptr);
        }
    }

    characters.erase(
        std::remove_if(
            characters.begin(),
            characters.end(),
            [](const std::unique_ptr<Character>& c)
            {
                return c->isDead();
            }),
        characters.end());
}

LevelPortal* Level::getLevelPortal(int index)
{
    if(index < 0 || index >= levelPortals.size())
        return nullptr;

    return levelPortals[index];
}