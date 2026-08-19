#include "dungeoncrawler.h"
#include "lootchest.h"
#include <QDebug>
#include <QMutexLocker>

void DungeonCrawler::turn(Input input)
{
    QMutexLocker locker(&mutex);

    if(!running)
        return;

    moveCharacter(player,input);

    getCurrentLevel().removeDeadCharacters();

    if(player->isDead())
        stopGame();
}

Character *DungeonCrawler::getPlayer() const
{
    return player;
}

bool DungeonCrawler::moveCharacter(Character *character, Input input)
{
    int dRow = 0;
    int dCol = 0;
    switch(input)
    {
    case Input::UP:
        dRow = -1;
        break;
    case Input::DOWN:
        dRow = 1;
        break;
    case Input::LEFT:
        dCol = -1;
        break;
    case Input::RIGHT:
        dCol = 1;
        break;
    default:
        return false;
    }

    Tile* current = character->getCurrentTile();
    int row = current->getRow();
    int col = current->getCol();
    Tile* target = getCurrentLevel().getTile(row + dRow, col + dCol);
    if(!target)
        return false;
    if(!current->onLeave(target, character))
    {
        return false;
    }
    Tile* actualTarget = target->onEnter(character);

    if(!actualTarget)
        return false;

    // Lootchest check
    if(dynamic_cast<LootChest*>(actualTarget) &&
        character->isPlayer())
    {
        current->setCharacter(nullptr);
        actualTarget->setCharacter(character);
        character->setCurrentTile(actualTarget);
        winGame();
        return true;
    }

    Character* defender = actualTarget->getCharacter();

    // //test
    // qDebug()
    //     << "Attacker =" << character
    //     << " Defender =" << defender
    //     << " Same =" << (character == defender)
    //     << " Target tile =" << actualTarget->getRow()
    //     << actualTarget->getCol();

    if(defender)
    {
        if(character->isPlayer() == defender->isPlayer())
        {
            return false;
        }
        character->attack(defender);
        if(defender->isDead())
        {
            actualTarget->setCharacter(nullptr);
        }
        else
        {
            defender->attack(character);
            if(character->isDead())
            {
                if(character->isPlayer())
                {
                    stopGame();
                }
                return false;
            }
            return false;
        }
    }

    if(LevelPortal* lp =
        dynamic_cast<LevelPortal*>(actualTarget))
    {
        current->setCharacter(nullptr);

        currentLevel = lp->getTargetLevel();

        LevelPortal* destination = getCurrentLevel().getLevelPortal(lp->getTargetPortal());

        if(!destination)
        {
            throw std::runtime_error(
                "Target level has no LevelPortal.");
        }

        destination->setCharacter(character);
        character->setCurrentTile(destination);

        return true;
    }
    // Normal movement
    current->setCharacter(nullptr);
    actualTarget->setCharacter(character);
    character->setCurrentTile(actualTarget);
    return true;
}

void DungeonCrawler::setPlayer(Character* p){
    player = p;
}

Level& DungeonCrawler::getLevel(int index)
{
    return level[index];
}

Level& DungeonCrawler::getCurrentLevel()
{
    return level[currentLevel];
}

void DungeonCrawler::setCurrentLevel(int level)
{
    currentLevel = level;
}

int DungeonCrawler::getCurrentLevel() const
{
    return currentLevel;
}

void DungeonCrawler::moveNPCs()
{
    QMutexLocker locker(&mutex);

    if(!running)
        return;

    auto& chars = getCurrentLevel().getCharacters();

    for(auto& c : chars)
    {
        if(c->isPlayer())
            continue;

        int dir = rand() % 4;

        switch(dir)
        {
        case 0:
            moveCharacter(c.get(), Input::UP);
            break;

        case 1:
            moveCharacter(c.get(), Input::DOWN);
            break;

        case 2:
            moveCharacter(c.get(), Input::LEFT);
            break;

        case 3:
            moveCharacter(c.get(), Input::RIGHT);
            break;
        }
    }

    getCurrentLevel().removeDeadCharacters();
}

bool DungeonCrawler::isRunning() const
{
    return running;
}

void DungeonCrawler::stopGame()
{
    running = false;
    playerWon = false;
}

void DungeonCrawler::winGame()
{
    running = false;
    playerWon = true;
}

bool DungeonCrawler::hasWon() const
{
    return playerWon;
}

QMutex& DungeonCrawler::getMutex()
{
    return mutex;
}