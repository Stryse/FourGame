#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <QTime>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent, int color[],
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
    void resetScore();
    //Color
    int getColor(int n) const;

signals:
    void timeEnded();

private:
    const QString playerName;
    const QTime startTime;
    QTime gameTime;
    int color[3];
    int score;
};

#endif // PLAYER_H
