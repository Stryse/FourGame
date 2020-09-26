#include "fourgamewindow.h"
#include "ui_fourgamewindow.h"

#include <QString>
#include <QColor>
#include <QColorDialog>
#include <QMessageBox>

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

    //PlayerName
    connect(ui->p1Edit,&QLineEdit::textChanged,this,[=](){ ui->playBtn->setEnabled(playReady());});
    connect(ui->p2Edit,&QLineEdit::textChanged,this,[=](){ ui->playBtn->setEnabled(playReady());});

    //PlayerColors
    connect(ui->p1ColorBtn,&QPushButton::clicked,this,[=]()
           {setPlayerColor(ui->p1ColorBtn); ui->playBtn->setEnabled(playReady());});
    connect(ui->p2ColorBtn,&QPushButton::clicked,this,[=]()
           {setPlayerColor(ui->p2ColorBtn); ui->playBtn->setEnabled(playReady());});
}

FourGameWindow::~FourGameWindow()
{
    delete ui;
    delete game;
    for(auto& p : players) delete p;
}

void FourGameWindow::startNewGame()
{
    players.clear();
    buttons.clear();

    //Player Name
    QString p1Name = (ui->p1Edit->text() != "") ? ui->p1Edit->text() : "Player 1";
    QString p2Name = (ui->p2Edit->text() != "") ? ui->p2Edit->text() : "Player 2";

    //Player Color
    QColor p1Color = ui->p1ColorBtn->palette().window().color();
    QColor p2Color = ui->p2ColorBtn->palette().window().color();

    //Populate Player
    players.push_back(new Player(this,p1Color,ui->timeEdit->time(),p1Name));
    players.push_back(new Player(this,p2Color,ui->timeEdit->time(),p2Name));

    //Setup Game Logic
    QTime gameTime = (ui->timeCheckBox->isChecked()) ? ui->timeEdit->time() : QTime(0,0,0,0);

    game = new GameLogic(this,gameTime,players);
    connect(game,SIGNAL(endGame()),this,SLOT(gameEnded()));
    connect(game,SIGNAL(playerChanged()),this,SLOT(on_playerChanged()));

    //Setup Game Environment
    initGameField();
}

void FourGameWindow::updateTime()
{
    ui->gameTimerDisplay->display(game->getActivePlayer()->subTime().toString());
}

void FourGameWindow::setPlayerColor(QPushButton* btn)
{
    QColor color = QColorDialog::getColor(btn->palette().window().color(),this,"Pick a player color");
    btn->setStyleSheet(QString("color: #fff;background-color: %0;").arg(color.name()));
}

void FourGameWindow::on_scored(int row, int col)
{
    //Disable
    buttons[row][col]->setEnabled(false);

    //Colorize
    buttons[row][col]->setStyleSheet(QString("font-size: 30px;color: #fff;background-color: %0;")
                                     .arg(game->getActivePlayer()->getColor().name()));

    //Update UI
    ui->p1scoreDisplay->display(game->getPlayer(0)->getScore());
    ui->p2scoreDisplay->display(game->getPlayer(1)->getScore());
}

void FourGameWindow::on_playerChanged()
{
    //Switch active player highlighting
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

    game->changeActivePlayer();
    game->checkWinCond();
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
}

void FourGameWindow::initGameField()
{
    //Hide Menu
    ui->menuVLayout->setVisible(false);

    //Set Player labels
    ui->p1statusLabel->setText(game->getPlayer(0)->getPlayerName());
    ui->p1statusLabel->setStyleSheet("color: rgb(252,238,40);");
    ui->p2statusLabel->setText(game->getPlayer(1)->getPlayerName());
    ui->p2statusLabel->setStyleSheet("color: rgb(211,211,211);");

    //Reset scores
    ui->p1scoreDisplay->display(game->getPlayer(0)->getScore());
    ui->p2scoreDisplay->display(game->getPlayer(1)->getScore());

    //Enable timer if selected
    if(ui->timeCheckBox->isChecked())
    {
        ui->gameTimerDisplay->display(game->getActivePlayer()->getGameTime().toString());
        ui->gameTimerDisplay->setVisible(true);
    }
    else
        ui->gameTimerDisplay->setVisible(false);

    //Populate ButtonGrid
    ui->gridLayout->setMargin(0);
    buttons.resize(game->rowCount);
    for(int i= 0; i < game->rowCount; ++i)
    {
       buttons[i].resize(game->colCount);
       for(int j = 0; j < game->colCount; ++j)
       {
           QPushButton* gameBtn = new QPushButton(ui->gridWidget);
           gameBtn->setText(QString::number(game->getFieldValue(i,j)));
           gameBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
           gameBtn->setStyleSheet("font-size: 20px;");

           ui->gridLayout->addWidget(gameBtn,i,j);
           buttons[i][j] = gameBtn;
           connect(gameBtn,&QPushButton::clicked,[=](){ gameButtonClick(i,j); });
       }
    }
}

void FourGameWindow::incrementButton(int row, int col)
{
    if(game->isValidField(row,col))
        buttons[row][col]->setText(QString::number(game->addPoint(row,col)));
}

bool FourGameWindow::playReady() const
{
    // Check if players' data conflicts
    QString p1Name = (ui->p1Edit->text() != "") ? ui->p1Edit->text() : "Player 1";
    QString p2Name = (ui->p2Edit->text() != "") ? ui->p2Edit->text() : "Player 2";

    return      p1Name != p2Name
            &&  ui->p1ColorBtn->palette().window().color() != ui->p2ColorBtn->palette().window().color();
}
