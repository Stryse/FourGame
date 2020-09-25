#include "fourgamewindow.h"
#include "ui_fourgamewindow.h"

FourGameWindow::FourGameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FourGameWindow)
{
    ui->setupUi(this);
    ui->gameTimerDisplay->setVisible(false);

    connect(ui->timeCheckBox,&QCheckBox::clicked,[=](bool isClicked){ ui->timeEdit->setEnabled(isClicked); });
    connect(ui->playBtn,SIGNAL(clicked()),this,SLOT(startNewGame()));
    connect(&gameTimer,SIGNAL(timeout()),this,SLOT(updateTime()));

}

FourGameWindow::~FourGameWindow()
{
    delete ui;
}

void FourGameWindow::startNewGame()
{
    //Hide Menu
    ui->menuVLayout->setVisible(false);

    //Set Player labels
    if(ui->p1Edit->text() != "") ui->p1statusLabel->setText(ui->p1Edit->text());
    else                         ui->p1statusLabel->setText("Player 1");

    if(ui->p2Edit->text() != "") ui->p2statusLabel->setText(ui->p2Edit->text());
    else                         ui->p2statusLabel->setText("Player 2");

    //Reset scores
    ui->p1scoreDisplay->display(0);
    ui->p2scoreDisplay->display(0);

    //Enable timer if selected
    if(ui->timeCheckBox->isChecked() && ui->timeEdit->time() != QTime(0,0,0,0))
    {
        gameTime = ui->timeEdit->time();

        ui->gameTimerDisplay->display(gameTime.toString());
        ui->gameTimerDisplay->setVisible(true);
        gameTimer.start(1000);
    }
    else
        ui->gameTimerDisplay->setVisible(false);


    initGameField();
}

void FourGameWindow::updateTime()
{
    gameTime = gameTime.addSecs(-1);
    ui->gameTimerDisplay->display(gameTime.toString());
    if(gameTime == QTime(0,0,0,0))
    {
        gameTimer.stop();
        emit endGame();
    }
}

void FourGameWindow::gameButtonClick(int row, int col)
{
    incrementButton(row,col);
}

void FourGameWindow::initGameField()
{
    for(int i= 0; i < rowCount; ++i)
    {
       for(int j = 0;j < colCount; ++j)
       {
           QPushButton* gameBtn = new QPushButton(ui->gridWidget);
           gameBtn->setText(QString::number(0));
           connect(gameBtn,&QPushButton::clicked,[=](){ gameButtonClick(i,j); });

           ui->gridLayout->addWidget(gameBtn,i,j,Qt::AlignCenter);
           buttons[i][j] = gameBtn;
       }
    }
}

void FourGameWindow::incrementButton(int row, int col)
{
    int value = buttons[row][col]->text().toInt();
    if(value < 3)
        buttons[row][col]->setText(QString::number(value+1));
    else
    {
        buttons[row][col]->setText(QString::number(value+1));
        buttons[row][col]->setEnabled(false);
    }
}
