#ifndef GAMELOGIC_H
#define GAMELOGIC_H

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
    //SIGNALS
    void scored(int row,int col);
    void playerChanged();
    void endGame();

public:
    // Public Fields
    const int rowCount;
    const int colCount;
    static const int maxFieldValue = 4;

public:

    // GameField methods
    int getFieldValue(int row, int col) const;
    int addPoint(int row, int col); // emits scored();
    bool isValidField(int row, int col) const;
    void checkWinCond(); // emits endGame();

    // Player methods
    Player* getPlayer(int n) const;
    Player* getActivePlayer() const;
    void changeActivePlayer(); // emits playerChanged();

private:

    // Private Fields
    QTimer gameTimer;
    QVector<Player*> players;
    Player* activePlayer;
    int activePInd;
    QVector<QVector<int>> fields;
};

#endif // GAMELOGIC_H
