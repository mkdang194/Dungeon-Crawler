#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "level.h"
#include <QResource>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QResizeEvent>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPainter>
#include "portal.h"
#include "player.h"
#include <QThread>
#include "npcworker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game()
{
    ui->setupUi(this);

    // Background
    ui->centralwidget->setStyleSheet("background-color:#111111;");
    ui->lblBloodFrame->setPixmap(QPixmap(":/bloody_frame.png"));
    ui->lblBloodFrame->setScaledContents(true);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // Start screen
    ui->lblStartScreen->setPixmap(QPixmap(":/startscreen.png"));
    ui->lblStartScreen->setScaledContents(true);
    // New Game button
    ui->btnNewGame->setIcon(QIcon(":/new_game_button.png"));
    ui->btnNewGame->setText("");
    ui->btnNewGame->setFixedSize(450, 150);

    ui->btnNewGame->setFlat(true);
    ui->btnNewGame->setStyleSheet("border:none;""background:transparent;");
    ui->btnNewGame->move(width()/2 - 225, height()/2 + 60);

    // Movement buttons
    ui->btnLeft->setIcon(QIcon(":/arrow_left.png"));
    ui->btnRight->setIcon(QIcon(":/arrow_right.png"));
    ui->btnUp->setIcon(QIcon(":/arrow_up.png"));
    ui->btnDown->setIcon(QIcon(":/arrow_down.png"));

    ui->btnLeft->setText("");
    ui->btnRight->setText("");
    ui->btnUp->setText("");
    ui->btnDown->setText("");

    ui->btnLeft->setFixedSize(50,50);
    ui->btnRight->setFixedSize(50,50);
    ui->btnUp->setFixedSize(50,50);
    ui->btnDown->setFixedSize(50,50);

    ui->btnLeft->setIconSize(QSize(48,48));
    ui->btnRight->setIconSize(QSize(48,48));
    ui->btnUp->setIconSize(QSize(48,48));
    ui->btnDown->setIconSize(QSize(48,48));

    ui->btnLeft->setFlat(true);
    ui->btnRight->setFlat(true);
    ui->btnUp->setFlat(true);
    ui->btnDown->setFlat(true);

    ui->btnLeft->setStyleSheet("border:none;background:transparent;");
    ui->btnRight->setStyleSheet( "border:none;background:transparent;");
    ui->btnUp->setStyleSheet("border:none;background:transparent;");
    ui->btnDown->setStyleSheet("border:none;background:transparent;");
    npcThread = new QThread(this);

    npcWorker = new NPCWorker(&m_game);

    npcWorker->moveToThread(npcThread);

    connect(npcThread, &QThread::started, npcWorker, &NPCWorker::process);

    connect(npcWorker,  &NPCWorker::npcMoved, this, &MainWindow::draw,  Qt::QueuedConnection);

    npcThread->start();
    showStartScreen();
}

MainWindow::~MainWindow()
{
    if(npcThread)
    {
        npcThread->quit();
        npcThread->wait();
    }

    delete ui;
}

void MainWindow::createGrid()
{
    auto& level = m_game.getCurrentLevel();

    ui->gridLayoutLevel->setSpacing(0);
    ui->gridLayoutLevel->setContentsMargins(0,0,0,0);

    for(int r=0; r<level.getRows(); r++)
    {
        for(int c=0; c<level.getCols(); c++)
        {
            QLabel* label = new QLabel();

            label->setFixedSize(TILE_SIZE, TILE_SIZE);
            label->setScaledContents(true);
            label->setAlignment(Qt::AlignCenter);

            ui->gridLayoutLevel->addWidget(label, r, c);
            m_labels.push_back(label);
        }
    }
}

void MainWindow::draw()
{
    QMutexLocker locker(&m_game.getMutex());
    if(m_labels.empty())
        return;
    auto& level = m_game.getCurrentLevel();
    int cols = level.getCols();
    for(int r = 0; r < level.getRows(); r++)
    {
        for(int c = 0; c < cols; c++)
        {
            Tile* tile = level.getTile(r, c);
            QLabel* label = m_labels[r * cols + c];

            QString tileImage;
            QString characterImage;
            std::string tex = tile->getTexture();

            if(tex == "_")
            {
                tileImage = ":/pit.png";
            }
            else if(tex == "<")
            {
                tileImage = ":/ramp.png";
            }
            else if(tex == "?")
            {
                tileImage = ":/switch.png";
            }
            else if(tex == "#")
            {
                tileImage = ":/wall1.png";
            }
            else if(tex == ".")
            {
                int id = (r + c) % 5 + 1;
                tileImage = QString(":/floor%1.png").arg(id);
            }
            else if(tex == "O")
            {
                int id = (m_portalFrame % 3) + 1;
                tileImage = QString(":/portal%1.png").arg(id);
            }
            else if(tex == "T")
            {
                tileImage = ":/door1.png";
            }
            else if(tex == "/")
            {
                tileImage = ":/door2.png";
            }
            else if(tex == "L")
            {
                tileImage = ":/lootchest.png";
            }
            else if(tex == "P")
            {
                tileImage = ":/levelportal.png";   // nếu có sprite riêng
            }
            if(tile->getCharacter())
            {
                Character* character = tile->getCharacter();

                if(character->isPlayer())
                {
                    QString frames[3] =
                        {
                            ":/char_right_1.png",
                            ":/char_right_2.png",
                            ":/char_right_3.png"
                        };

                    characterImage = frames[m_playerFrame];
                }
                else
                {
                    characterImage = ":/zombie_left.png";
                }
            }
            QPixmap result(m_tileSize, m_tileSize);
            result.fill(Qt::transparent);
            QPainter painter(&result);

            if(!tileImage.isEmpty())
            {
                QPixmap tilePix(tileImage);
                 painter.drawPixmap(0,  0, tilePix.scaled( m_tileSize,
                 m_tileSize,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation));
            }

            if(!characterImage.isEmpty())
            {
                QPixmap charPix(characterImage);

                painter.drawPixmap(0, 0,
                    charPix.scaled(
                        m_tileSize,
                        m_tileSize,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
            }
            painter.end();
            label->setPixmap(
                result.scaled(
                    m_tileSize,
                    m_tileSize,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
            label->setText("");
        }
    }

    updateStatusBar();
}

void MainWindow::showStartScreen()
{
    ui->lblStartScreen->show();
    ui->btnNewGame->show();

    ui->lblBloodFrame->hide();
    ui->gridLayoutWidget->hide();

    ui->btnLeft->hide();
    ui->btnRight->hide();
    ui->btnUp->hide();
    ui->btnDown->hide();
}

void MainWindow::startGame()
{
    ui->lblStartScreen->hide();
    ui->btnNewGame->hide();

    ui->lblBloodFrame->show();
    ui->gridLayoutWidget->show();

    ui->btnLeft->show();
    ui->btnRight->show();
    ui->btnUp->show();
    ui->btnDown->show();

    // Load levels from file
    m_game.getLevel(0).loadFromFile("level1.txt");
    m_game.getLevel(1).loadFromFile("level2.txt");

    m_game.setCurrentLevel(0);
    auto player = std::make_unique<Player>(10,8);
    Character* playerPtr = player.get();
    m_game.getLevel(0).placeCharacter(std::move(player), 1, 2);

    m_game.setPlayer(playerPtr);
    if(m_labels.empty())
        createGrid();

    updateLayout();
    QApplication::processEvents();
    draw();
    updateStatusBar();
    setFocus();
}

void MainWindow::on_btnLeft_clicked()
{
    m_playerFrame = (m_playerFrame + 1) % 3;
    m_game.turn(Input::LEFT);
    if(!m_game.isRunning())
    {
        if(m_game.hasWon())
        {
            QMessageBox::information(this," Victory", "You found the loot!");
        }
        else
        {
            QMessageBox::information(this, "Game Over", "You died!");
        }
        close();
        return;
    }
    updateStatusBar();
    draw();
}


void MainWindow::on_btnRight_clicked()
{
    m_playerFrame = (m_playerFrame + 1) % 3;
    m_game.turn(Input::RIGHT);
    if(!m_game.isRunning())
    {
        if(m_game.hasWon())
        {
            QMessageBox::information(this," Victory", "You found the loot!");
        }
        else
        {
            QMessageBox::information(this, "Game Over", "You died!");
        }
        close();
        return;
    }
    updateStatusBar();
    draw();
}


void MainWindow::on_btnUp_clicked()
{
    m_playerFrame = (m_playerFrame + 1) % 3;
    m_game.turn(Input::UP);
    if(!m_game.isRunning())
    {
        if(m_game.hasWon())
        {
            QMessageBox::information(this," Victory", "You found the loot!");
        }
        else
        {
            QMessageBox::information(this, "Game Over", "You died!");
        }
        close();
        return;
    }
    updateStatusBar();
    draw();
}


void MainWindow::on_btnDown_clicked()
{
    m_playerFrame = (m_playerFrame + 1) % 3;
    m_game.turn(Input::DOWN);
    if(!m_game.isRunning())
    {
        if(m_game.hasWon())
        {
            QMessageBox::information(this," Victory", "You found the loot!");
        }
        else
        {
            QMessageBox::information(this, "Game Over", "You died!");
        }
        close();
        return;
    }
    updateStatusBar();
    draw();
}


void MainWindow::on_btnNewGame_clicked()
{
    startGame();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // Fullscreen overlays
    ui->lblStartScreen->setGeometry(ui->centralwidget->rect());
    ui->lblBloodFrame->setGeometry(ui->centralwidget->rect());
    // New Game button
    int btnW = width() * 0.35;
    int btnH = btnW * 0.30;
    ui->btnNewGame->setFixedSize(btnW,btnH);
    ui->btnNewGame->setIconSize(QSize(btnW, btnH));
    ui->btnNewGame->move( width()/2 - btnW/2,height()*0.70);

    // Update map and buttons
    updateLayout();
    if(!m_labels.empty())
    {
        draw();
    }
    ui->lblBloodFrame->lower();
    ui->gridLayoutWidget->raise();

    ui->btnLeft->raise();
    ui->btnRight->raise();
    ui->btnUp->raise();
    ui->btnDown->raise();
}

void MainWindow::updateLayout()
{
    if(m_labels.empty())
        return;

    int cols = m_game.getCurrentLevel().getCols();

    int rows = m_game.getCurrentLevel().getRows();
    // MAP SCALE
    int availableW = ui->centralwidget->width() * 0.70;
    int availableH = ui->centralwidget->height() * 0.60;
    m_tileSize =qMin(availableW / cols,availableH / rows);
    int mapWidth =cols * m_tileSize;
    int mapHeight =rows * m_tileSize;
    int x =(ui->centralwidget->width()- mapWidth) / 2;
    int y =(ui->centralwidget->height()- mapHeight) / 2 + 40;
    ui->gridLayoutWidget->setGeometry(x, y, mapWidth, mapHeight);
    for(QLabel* label : m_labels)
    {
        label->setFixedSize(m_tileSize, m_tileSize);
    }

    // D-PAD SCALE
    int moveBtnSize = qMax(50, width()/20);
    int margin = width()*0.02;
    int baseX = margin;
    int baseY = height() - moveBtnSize*3  - margin;

    ui->btnLeft->setGeometry(baseX,  baseY + moveBtnSize, moveBtnSize,  moveBtnSize);
    ui->btnRight->setGeometry(baseX + moveBtnSize*2, baseY + moveBtnSize,  moveBtnSize, moveBtnSize);
    ui->btnUp->setGeometry(baseX + moveBtnSize,  baseY,  moveBtnSize, moveBtnSize);
    ui->btnDown->setGeometry(baseX + moveBtnSize,  baseY + moveBtnSize*2, moveBtnSize, moveBtnSize);

    QSize iconSize(moveBtnSize*0.8, moveBtnSize*0.8);

    ui->btnLeft->setIconSize(iconSize);
    ui->btnRight->setIconSize(iconSize);
    ui->btnUp->setIconSize(iconSize);
    ui->btnDown->setIconSize(iconSize);
}

void MainWindow::updateStatusBar()
{
    Character* player = m_game.getPlayer();
    if(player == nullptr)
        return;
    QString text =
        QString("HP: %1/%2   STR: %3   STA: %4")
            .arg(player->getHitpoints())
            .arg(player->getMaxHP())
            .arg(player->getStrength())
            .arg(player->getStamina());
    statusBar()->showMessage(text);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        m_playerFrame = (m_playerFrame + 1) % 3;
        m_game.turn(Input::LEFT);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_playerFrame = (m_playerFrame + 1) % 3;
        m_game.turn(Input::RIGHT);
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        m_playerFrame = (m_playerFrame + 1) % 3;
        m_game.turn(Input::UP);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_playerFrame = (m_playerFrame + 1) % 3;
        m_game.turn(Input::DOWN);
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }
    if(!m_game.isRunning())
    {
        if(m_game.hasWon())
        {
            QMessageBox::information(this," Victory", "You found the loot!");
        }
        else
        {
            QMessageBox::information(this, "Game Over", "You died!");
        }
        close();
        return;
    }
    updateStatusBar();
    draw();
}