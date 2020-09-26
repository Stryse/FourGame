#include "player.h"

Player::Player(QObject *parent, int clr[],
               const QTime& time, const QString& pName)
    : QObject(parent)
    , playerName(pName)
    , startTime(time)
    , gameTime(time)
    , score(0)
{
    for(int i = 0; i < 3; ++i) color[i] = clr[i];
}

const QTime& Player::getGameTime() const
{
    return gameTime;
}

const QTime& Player::subTime()
{
    gameTime = gameTime.addSecs(-1);
    if(gameTime == QTime(0,0,0,0))
    {
        emit timeEnded();
        gameTime = startTime;
    }

    return gameTime;
}

void Player::resetTime()
{
    gameTime = startTime;
}

const QString& Player::getPlayerName() const
{
    return playerName;
}

int Player::getScore() const
{
    return score;
}

int Player::addScore()
{
    return ++score;
}

void Player::resetScore()
{
    score = 0;
}

int Player::getColor(int n) const
{
    Q_ASSERT(n <= 3 && n >= 0);
    return color[n];
}
