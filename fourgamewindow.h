#ifndef FOURGAMEWINDOW_H
#define FOURGAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class FourGameWindow; }
QT_END_NAMESPACE

class FourGameWindow : public QWidget
{
    Q_OBJECT

public:
    FourGameWindow(QWidget* parent = nullptr);
    ~FourGameWindow();    

//SIGNALS
public: signals:
    void endGame();

//SLOTS
private slots:
    void startNewGame();
    void updateTime();
    void gameButtonClick(int row,int col);

//FIELDS
private:
    Ui::FourGameWindow* ui;
    QTimer gameTimer;
    QTime gameTime;

    static const int rowCount = 5;
    static const int colCount = 5;
    QPushButton* buttons[rowCount][colCount];

//METHODS
    void initGameField();
    void incrementButton(int row,int col);

};
#endif // FOURGAMEWINDOW_H
