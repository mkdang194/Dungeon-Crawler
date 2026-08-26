#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dungeoncrawler.h"
#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include "npcworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static constexpr int TILE_SIZE = 45;
    ~MainWindow() override;
    QThread* npcThread = nullptr;
    NPCWorker* npcWorker = nullptr;

private slots:
    void on_btnLeft_clicked();

    void on_btnRight_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnNewGame_clicked();

private:
    Ui::MainWindow *ui;
    DungeonCrawler m_game;

    std::vector<QLabel*> m_labels;
    int m_playerFrame = 0;
    int m_portalFrame = 0;
    int m_tileSize = 32;
    void createGrid();
    void draw();

    void showStartScreen();
    void startGame();
    void updateLayout();
    void updateStatusBar();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};
#endif // MAINWINDOW_H
