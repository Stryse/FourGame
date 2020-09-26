#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QColor>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent, const QColor& color,
                    const QTime& time, const QString& pName);

    //Name
    const QString& getPlayerName() const;
    //Time
    const QTime& getGameTime() const;
    const QTime& subTime();
    void resetTime();
    //Score
    int getScore() const;
    int addScore();
    //Color
    const QColor& getColor() const;

signals:
    void timeEnded();

private:
    const QString playerName;
    const QTime startTime;
    QTime gameTime;
    QColor color;
    int score;
};

#endif // PLAYER_H
