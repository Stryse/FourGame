#include "player.h"

Player::Player(QObject *parent, const QColor& color,
               const QTime& time, const QString& pName)
    : QObject(parent)
    , playerName(pName)
    , startTime(time)
    , gameTime(time)
    , color(color)
    , score(0)
{
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

const QColor &Player::getColor() const
{
    return color;
}
