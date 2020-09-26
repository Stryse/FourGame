#include "fourgamewindow.h"
#include "ui_fourgamewindow.h"

#include <QMessageBox>
#include <QStyle>
#include <QString>

FourGameWindow::FourGameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FourGameWindow)
    , game(nullptr)
{
    ui->setupUi(this);
    ui->gameTimerDisplay->setVisible(false);

    connect(ui->timeCheckBox,&QCheckBox::clicked,[=](bool isClicked){ ui->timeEdit->setEnabled(isClicked); });
    connect(ui->playBtn,SIGNAL(clicked()),this,SLOT(startNewGame()));
}

FourGameWindow::~FourGameWindow()
{
    delete ui;
    delete game;
}

void FourGameWindow::startNewGame()
{

    int p1c[3] = {255,0,0};
    int p2c[3] = {0,0,255};

    QString p1Name = (ui->p1Edit->text() != "") ? ui->p1Edit->text() : "Player 1";
    QString p2Name = (ui->p2Edit->text() != "") ? ui->p2Edit->text() : "Player 2";

    QVector<Player*> players {
        new Player(game,p1c,ui->timeEdit->time(),p1Name),
        new Player(game,p2c,ui->timeEdit->time(),p2Name)
    };

    game = new GameLogic(this,ui->timeEdit->time(),players);
    connect(game,SIGNAL(endGame()),this,SLOT(gameEnded()));

    //Hide Menu
    ui->menuVLayout->setVisible(false);

    //Set Player labels
    ui->p1statusLabel->setText(game->getPlayer(0)->getPlayerName());
    ui->p2statusLabel->setText(game->getPlayer(1)->getPlayerName());

    //Reset scores
    ui->p1scoreDisplay->display(game->getPlayer(0)->getScore());
    ui->p2scoreDisplay->display(game->getPlayer(1)->getScore());

    //Enable timer if selected
    if(ui->timeCheckBox->isChecked() && ui->timeEdit->time() != QTime(0,0,0,0))
    {
        ui->gameTimerDisplay->display(game->getActivePlayer()->getGameTime().toString());
        ui->gameTimerDisplay->setVisible(true);
    }
    else
        ui->gameTimerDisplay->setVisible(false);


    initGameField();
}

void FourGameWindow::updateTime()
{
    ui->gameTimerDisplay->display(game->getActivePlayer()->subTime().toString());
}

void FourGameWindow::on_scored(int row, int col)
{
    buttons[row][col]->setEnabled(false);

    QString r = QString::number(game->getActivePlayer()->getColor(0));
    QString g = QString::number(game->getActivePlayer()->getColor(1));
    QString b = QString::number(game->getActivePlayer()->getColor(2));
    buttons[row][col]->setStyleSheet(QString("background-color: rgb(%0,%1,%2);").arg(r,g,b));

    ui->p1scoreDisplay->display(game->getPlayer(0)->getScore());
    ui->p2scoreDisplay->display(game->getPlayer(1)->getScore());
}

void FourGameWindow::gameButtonClick(int row, int col)
{
    incrementButton(row,col);
    game->changeActivePlayer();
}

void FourGameWindow::gameEnded()
{
    ui->gameTimerDisplay->setVisible(false);
    QMessageBox msgBox;
    msgBox.setText("Game Over");
    msgBox.setWindowTitle("Game Over!");
    msgBox.exec();
}

void FourGameWindow::initGameField()
{
    buttons.resize(game->rowCount);
    for(int i= 0; i < game->rowCount; ++i)
    {
       buttons[i].resize(game->colCount);
       for(int j = 0; j < game->colCount; ++j)
       {
           QPushButton* gameBtn = new QPushButton(ui->gridWidget);
           gameBtn->setText(QString::number(game->getFieldValue(i,j)));
           connect(gameBtn,&QPushButton::clicked,[=](){ gameButtonClick(i,j); });

           ui->gridLayout->addWidget(gameBtn,i,j,Qt::AlignCenter);
           buttons[i][j] = gameBtn;
       }
    }
}

void FourGameWindow::incrementButton(int row, int col)
{
    buttons[row][col]->setText(QString::number(game->addPoint(row,col)));
}
