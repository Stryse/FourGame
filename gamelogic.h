#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#define PLAYERCOUNT 2

#include <QObject>
#include <QTimer>
#include <QVector>

#include "player.h"


class GameLogic : public QObject
{
    Q_OBJECT
public:
    //CTOR
    explicit GameLogic(QObject *parent, const QTime& time,
                       const QVector<Player*>& players ,int rowCount = 5,int colCount = 5);
    ~GameLogic();

public: signals:
    void timeChanged();
    void scored(int row,int col);

public:
    // Public Fields
    const int rowCount;
    const int colCount;
    static const int maxFieldValue = 4;

public:

    // GameField methods
    int getFieldValue(int row, int col) const;
    int addPoint(int row, int col);

    // Player methods
    Player* getPlayer(int n) const;
    Player* getActivePlayer() const;
    void changeActivePlayer();


signals:
    void endGame();

private:

    // Private fields
    QTimer gameTimer;
    QVector<Player*> players;
    Player* activePlayer;
    int activePInd;
    QVector<QVector<int>> fields;
};

#endif // GAMELOGIC_H
