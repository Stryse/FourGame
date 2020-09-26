#ifndef FOURGAMEWINDOW_H
#define FOURGAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QPushButton>

#include "gamelogic.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FourGameWindow; }
QT_END_NAMESPACE

class FourGameWindow : public QWidget
{
    Q_OBJECT

public:
//CTOR
    FourGameWindow(QWidget* parent = nullptr);
    ~FourGameWindow();    

//SIGNALS
public: signals:
    void endGame();

//SLOTS
private slots:
    void startNewGame();
    void updateTime();
    void on_scored(int row, int col);
    void gameButtonClick(int row,int col);
    void gameEnded();

//FIELDS
private:
    Ui::FourGameWindow* ui;
    GameLogic* game;

    QVector<QVector<QPushButton*>> buttons;

//METHODS
    void initGameField();
    void incrementButton(int row,int col);

};
#endif // FOURGAMEWINDOW_H
