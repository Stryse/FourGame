#include "fourgamewindow.h"
#include "ui_fourgamewindow.h"

#include <QMessageBox>
#include <QString>
#include <QColor>
#include <QColorDialog>

FourGameWindow::FourGameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FourGameWindow)
    , game(nullptr)
{
    ui->setupUi(this);
    ui->gameTimerDisplay->setVisible(false);

    //Checkbox
    connect(ui->timeCheckBox,&QCheckBox::clicked,[=](bool isClicked){ ui->timeEdit->setEnabled(isClicked); });
    //PlayStart
    connect(ui->playBtn,SIGNAL(clicked()),this,SLOT(startNewGame()));
    //Colors
    connect(ui->p1ColorBtn,&QPushButton::clicked,this,[=](){ setPlayerColor(ui->p1ColorBtn);});
    connect(ui->p2ColorBtn,&QPushButton::clicked,this,[=](){ setPlayerColor(ui->p2ColorBtn);});
}

FourGameWindow::~FourGameWindow()
{
    delete ui;
    delete game;
}

void FourGameWindow::startNewGame()
{
    players.clear();
    buttons.clear();

    //Player Name
    QString p1Name = (ui->p1Edit->text() != "") ? ui->p1Edit->text() : "Player 1";
    QString p2Name = (ui->p2Edit->text() != "") ? ui->p2Edit->text() : "Player 2";

    //Player Color
    QColor p1Color = ui->p1ColorBtn->palette().background().color();
    QColor p2Color = ui->p2ColorBtn->palette().background().color();
    int p1c[] = {p1Color.red(),p1Color.green(),p1Color.blue()};
    int p2c[] = {p2Color.red(),p2Color.green(),p2Color.blue()};

    //Populate Player
    players.push_back(new Player(this,p1c,ui->timeEdit->time(),p1Name));
    players.push_back(new Player(this,p2c,ui->timeEdit->time(),p2Name));

    game = new GameLogic(this,ui->timeEdit->time(),players);
    connect(game,SIGNAL(endGame()),this,SLOT(gameEnded()));
    connect(game,SIGNAL(playerChanged()),this,SLOT(on_playerChanged()));

    initGameField();
}

void FourGameWindow::updateTime()
{
    ui->gameTimerDisplay->display(game->getActivePlayer()->subTime().toString());
}

void FourGameWindow::setPlayerColor(QPushButton* btn)
{
    QColor color = QColorDialog::getColor(btn->palette().background().color(),this,"Pick a player color");
    btn->setStyleSheet(QString("color: #fff;background-color: %0;").arg(color.name()));
}

void FourGameWindow::on_scored(int row, int col)
{
    buttons[row][col]->setEnabled(false);

    QString r = QString::number(game->getActivePlayer()->getColor(0));
    QString g = QString::number(game->getActivePlayer()->getColor(1));
    QString b = QString::number(game->getActivePlayer()->getColor(2));
    buttons[row][col]->setStyleSheet(QString("color: #fff;background-color: rgb(%0,%1,%2);").arg(r,g,b));

    ui->p1scoreDisplay->display(game->getPlayer(0)->getScore());
    ui->p2scoreDisplay->display(game->getPlayer(1)->getScore());
}

void FourGameWindow::on_playerChanged()
{
    if(game->getActivePlayer()->getPlayerName() == ui->p1statusLabel->text())
    {
        ui->p1statusLabel->setStyleSheet("color: rgb(252,238,40);");
        ui->p2statusLabel->setStyleSheet("color: rgb(211,211,211);");
    }
    else
    {
        ui->p2statusLabel->setStyleSheet("color: rgb(252,238,40);");
        ui->p1statusLabel->setStyleSheet("color: rgb(211,211,211);");
    }
}

void FourGameWindow::gameButtonClick(int row, int col)
{
    for(int i = row-1; i <= row+1; ++i)
        for(int j = col-1; j <= col+1; ++j)
            incrementButton(i,j);
    game->checkWinCond();
    game->changeActivePlayer();
}

void FourGameWindow::gameEnded()
{
    //Show Victory screen
    ui->gameTimerDisplay->setVisible(false);
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over!");
    msgBox.setText("Game Over");

    if(game->getPlayer(0)->getScore() > game->getPlayer(1)->getScore())
        msgBox.setText(QString("Game Over! %0 has won! %1-%2")
              .arg(game->getPlayer(0)->getPlayerName(),
                   QString::number(game->getPlayer(0)->getScore()),
                   QString::number(game->getPlayer(1)->getScore())));
    else
        msgBox.setText(QString("Game Over! %0 has won! %1-%2")
              .arg(game->getPlayer(1)->getPlayerName(),
                   QString::number(game->getPlayer(1)->getScore()),
                   QString::number(game->getPlayer(0)->getScore())));

    msgBox.exec();

    //Delete previous game instance
    delete game;
    for(auto& p : players) delete p;

    //Start over
    startNewGame();
    game->changeActivePlayer();
}

void FourGameWindow::initGameField()
{
    //Hide Menu
    ui->menuVLayout->setVisible(false);

    //Set Player labels
    ui->p1statusLabel->setText(game->getPlayer(0)->getPlayerName());
    ui->p1statusLabel->setStyleSheet("color: rgb(252,238,40);");
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

    //Populate ButtonGrid
    buttons.resize(game->rowCount);
    for(int i= 0; i < game->rowCount; ++i)
    {
       buttons[i].resize(game->colCount);
       for(int j = 0; j < game->colCount; ++j)
       {
           QPushButton* gameBtn = new QPushButton(ui->gridWidget);
           buttons[i][j] = gameBtn;
           gameBtn->setText(QString::number(game->getFieldValue(i,j)));
           ui->gridLayout->addWidget(gameBtn,i,j,Qt::AlignCenter);
           connect(gameBtn,&QPushButton::clicked,[=](){ gameButtonClick(i,j); });
       }
    }
}

void FourGameWindow::incrementButton(int row, int col)
{
    if(game->isValidField(row,col))
        buttons[row][col]->setText(QString::number(game->addPoint(row,col)));
}
