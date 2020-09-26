#include "gamelogic.h"

GameLogic::GameLogic(QObject *parent, const QTime& time,
                     const QVector<Player*>& players, int rowCount, int colCount)
  : QObject(parent)
  , rowCount(rowCount),colCount(colCount)
  , players(players)
  , activePlayer(getPlayer(0)) , activePInd(0)
  , fields(rowCount)
{
    //Setup Players
    for(auto& player : players)
        connect(player,&Player::timeEnded,[=](){ changeActivePlayer();});

    //Setup fields
    for(auto& field : fields)
    {
        field.resize(colCount);
        field.fill(0,field.size());
    }
    connect(this,SIGNAL(scored(int,int)),parent,SLOT(on_scored(int,int)));

    //Setup GameTimer
    if(time != QTime(0,0,0,0))
    {
        gameTimer.start(1000);
        connect(&gameTimer,SIGNAL(timeout()),parent,SLOT(updateTime()));
    }
}

GameLogic::~GameLogic()
{
}

int GameLogic::getFieldValue(int row, int col) const
{
    return fields[row][col];
}

int GameLogic::addPoint(int row, int col)
{
    //SKIP IF REACHED
    if(fields[row][col] == GameLogic::maxFieldValue)
        return fields[row][col];

    //Increment field
    ++fields[row][col];

    //Check if scored
    if(fields[row][col] == GameLogic::maxFieldValue)
    {
        getActivePlayer()->addScore();
        emit scored(row,col);
    }
    return fields[row][col];
}

bool GameLogic::isValidField(int row, int col) const
{
    return row >= 0 && row < rowCount
            && col >= 0 && col < colCount;
}

void GameLogic::checkWinCond()
{
    int scoreSum = 0;
    for(auto& p : players)
        scoreSum += p->getScore();

    if(scoreSum == rowCount * colCount)
        emit endGame();
}

Player *GameLogic::getPlayer(int n) const
{
    return players[n];
}

Player *GameLogic::getActivePlayer() const
{
    return activePlayer;
}

void GameLogic::changeActivePlayer()
{
    activePlayer->resetTime();
    activePlayer = players[(activePInd+1)%players.size()];
    activePInd = (activePInd+1)%players.size();
    emit playerChanged();
}
