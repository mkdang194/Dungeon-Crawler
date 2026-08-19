#ifndef DUNGEONCRAWLER_H
#define DUNGEONCRAWLER_H
#include "Input.h"
#include "Level.h"
#include <QMutex>

class Character;

class DungeonCrawler{
private:
    std::vector<Level> level;
    Character* player = nullptr;
    int currentLevel = 0;
    bool running = true;
    bool playerWon = false;
    bool moveCharacter(Character* character, Input input);
    QMutex mutex;

public:
    DungeonCrawler()
    {
        level.emplace_back(10,10);
        level.emplace_back(10,10);
    }
    // DungeonCrawler(int rows, int cols)  : level(rows, cols) {}
    void setPlayer(Character* p);
    Level& getLevel(int index);
    Level& getCurrentLevel();
    void turn(Input input);
    Character* getPlayer() const;
    void setCurrentLevel(int level);

    int getCurrentLevel() const;
    void moveNPCs();
    bool isRunning() const;
    void stopGame();
    void winGame();
    bool hasWon() const;
    QMutex& getMutex();
};

#endif // DUNGEONCRAWLER_H
