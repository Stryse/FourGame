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
    //Score
    int getScore() const;
    int addScore();
    //Color
    int getColor(int n) const;

signals:
    void timeEnded();

private:
    const QString playerName;
    int color[3];
    int score;
    QTime gameTime;

};

#endif // PLAYER_H
