#include "npcworker.h"
#include "dungeoncrawler.h"

#include <QThread>

NPCWorker::NPCWorker(DungeonCrawler* g)
    : game(g)
{
}

void NPCWorker::process()
{
    while(game->isRunning())
    {
        QThread::sleep(1);

        game->moveNPCs();

        emit npcMoved();
    }
}