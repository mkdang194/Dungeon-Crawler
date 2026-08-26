#ifndef NPCWORKER_H
#define NPCWORKER_H

#include <QObject>

class DungeonCrawler;

class NPCWorker : public QObject
{
    Q_OBJECT

private:
    DungeonCrawler* game;

public:
    explicit NPCWorker(DungeonCrawler* g);

public slots:
    void process();

signals:
    void npcMoved();
};

#endif