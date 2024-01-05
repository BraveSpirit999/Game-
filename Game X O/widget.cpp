#include "widget.h"
#include "./ui_widget.h"
#include<QTabBar>
#include"stylehelper.h"
#include<QStyleOption>
#include<QDebug>
#include<QFontDatabase>
#include<QGridLayout>
#include<time.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ConfigurationTabWidget();
    setInterfaceStyle();
    addFonts();
    ConfigurationGameAreaButtons();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::onComputerSlot);
}

Widget::~Widget()
{
    delete ui;
}
/*Применяем правила QSS*/
void Widget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
/*Настраиваем интерфейс программы*/
void Widget::setInterfaceStyle()
{
    this->setStyleSheet(StyleHelper::getDarkMainWidgetStyle());
    ui->aboutButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
    ui->startButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
    ui->leftButton->setStyleSheet(StyleHelper::getDarkLeftButtonActiveStyle());
    ui->rightButton->setStyleSheet(StyleHelper::getDarkRightButtonStyle());
    ui->tabWidget->setStyleSheet(StyleHelper::getTabWidgetStyle());
    ui->darkButton->setStyleSheet(StyleHelper::getGameStyleButton());
    ui->lightButton->setStyleSheet(StyleHelper::getGameStyleButton());
    ui->label->setStyleSheet(StyleHelper::getDarkNameGameStyle());

    ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
    ui->messageLabel->setText("Ходят крестики");

    setGameAreaButtonStyle();
    ui->messageLabel->setText("");
    ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());
    ui->label_3->setStyleSheet(StyleHelper::getAboutTextStyle());
}
/*Переключаем выбор крестики нолики*/
void Widget::changeButtonStatus(int num)
{
    if(num==1){
        if(interface==true){
            ui->leftButton->setStyleSheet(StyleHelper::getDarkLeftButtonActiveStyle());
            ui->rightButton->setStyleSheet(StyleHelper::getDarkRightButtonStyle());
        }
        else{
            ui->leftButton->setStyleSheet(StyleHelper::getLightLeftButtonActiveStyle());
            ui->rightButton->setStyleSheet(StyleHelper::getLightRightButtonStyle());
        }
    }
    else{
        if(interface==true){
            ui->leftButton->setStyleSheet(StyleHelper::getDarkLeftButtonStyle());
            ui->rightButton->setStyleSheet(StyleHelper::getDarkRightButtonActiveStyle());
        }
        else{
            ui->leftButton->setStyleSheet(StyleHelper::getLightLeftButtonStyle());
            ui->rightButton->setStyleSheet(StyleHelper::getLightRightButtonActiveStyle());
        }
    }
}
/*Скрываем второе окно виджета*/
void Widget::ConfigurationTabWidget()
{
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setMaximumHeight(320);
    ui->tabWidget->setCurrentIndex(0);
}
/*Добавляем шрифты*/
void Widget::addFonts()
{
    QFontDatabase::addApplicationFont(":/resourse/fonts/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/resourse/fonts/Roboto-MediumItalic.ttf");
}

void Widget::setGameAreaButtonStyle()
{
    QString style = StyleHelper::getBlankButtonStyle();//пустая кнопка
    for(int row=0;row<3;++row){
        for(int colum=0;colum<3;++colum){
            changeButtonStyle(row,colum,style);
        }
    }

}
//Начало игры
void Widget::start()
{
    setGameAreaButtonStyle();
    for(int r=0;r<3;++r){
        for(int c=0;c<3;++c){
            gameArea[r][c]='-';
        }
    }
    progress = 0;
    gameStart = true;
    stop = false;
    if(player!='X')
        ComputerInGame();
}

void Widget::LockPlayer()
{
    if(player=='X')
        playerLocked = false;
    else
        playerLocked = true;
}

void Widget::changeButtonStyle(int row, int colum, QString style)
{
    QGridLayout *grid = qobject_cast<QGridLayout*>(ui->tab->layout()); /*Указатель на компоновщик*/
    QPushButton *bth = qobject_cast<QPushButton*>(grid->itemAtPosition(row,colum)->widget());
    bth->setStyleSheet(style);
}

void Widget::ConfigurationGameAreaButtons() // получаем координаты клика по кнопке
{
    QGridLayout *grid = qobject_cast<QGridLayout*>(ui->tab->layout());
    for(int row=0;row<3;++row){
        for(int colum=0;colum<3;++colum){
            QPushButton *bth = qobject_cast<QPushButton*>(grid->itemAtPosition(row,colum)->widget());
            bth->setProperty("row",row);
            bth->setProperty("colum",colum);
            connect(bth, &QPushButton::clicked, this, &Widget::onGameAreaButtonClicked);
        }
    }
}
/*Выбрал крестик*/
void Widget::on_leftButton_clicked()
{
    changeButtonStatus(1);
    player = 'X';
}
/*Выбрал нолик*/
void Widget::on_rightButton_clicked()
{
    changeButtonStatus(2);
    player = '0';
}


void Widget::on_startButton_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    if(gameStart){
        playerLocked = true;
        ui->startButton->setText("Играть");
        if(interface == true)
            ui->startButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
        else
            ui->startButton->setStyleSheet(StyleHelper::getLightStartButtonStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;
        ui->messageLabel->setText("Проиграл...");
        ui->messageLabel->setStyleSheet(StyleHelper::getLostMessageStyle());
    }
    else{
        ui->messageLabel->setText("Напряги мозги!!!");
        ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());
        start();
        LockPlayer();
        ui->startButton->setText("Сдаюсь...");
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonGameStyle());
        ui->leftButton->setDisabled(true);
        ui->rightButton->setDisabled(true);

    }
}

void Widget::onGameAreaButtonClicked() // Твой ход
{
    if(!playerLocked){
        QPushButton *bth = qobject_cast<QPushButton*>(sender());
        int row = bth->property("row").toInt();
        int colum = bth->property("colum").toInt();
        QString style;
        if(gameArea[row][colum]!='-'){
            return;
        }
        if(player=='X'){
            style = StyleHelper::getCrossNormalStyle();
            gameArea[row][colum] = 'X';
        }
        else{
            style = StyleHelper::getZeroNormalStyle();
            gameArea[row][colum] = '0';
        }
        changeButtonStyle(row,colum,style);
        playerLocked = true;
        progress++;
        chekGameStop();
        endGame();
        ComputerInGame();
    }
}

void Widget::ComputerInGame()
{
    if(stop)
        return;
    srand(time(0));
    int index = rand()%4;
    QStringList list = {"Мой ход","Так так так...","Хм...сложно","Думаю как тебе поддаться!!!"};
    ui->messageLabel->setText(list.at(index));
    timer->start(2000);
}

void Widget::chekGameStop()
{
    winner = '-';
    QString style;
    char symbols[2]={'X','0'};
    for(int i=0;i<2;++i)
    {
        for(int row = 0; row<3; ++row)
        {
            if((gameArea[row][0]==symbols[i]) and (gameArea[row][1]==symbols[i]) and (gameArea[row][2]==symbols[i]))
            {
                stop = true;
                winner = symbols[i];
                EndGameStyle(row,0);
                EndGameStyle(row,1);
                EndGameStyle(row,2);
                return;
            }
        }
        for(int colum = 0; colum<3; ++colum)
        {
            if((gameArea[0][colum]==symbols[i]) and (gameArea[1][colum]==symbols[i]) and (gameArea[2][colum]==symbols[i]))
            {
                stop = true;
                winner = symbols[i];
                EndGameStyle(0,colum);
                EndGameStyle(1,colum);
                EndGameStyle(2,colum);
                return;
            }
        }
        if((gameArea[0][0]==symbols[i]) and (gameArea[1][1]==symbols[i]) and (gameArea[2][2]==symbols[i]))
        {
            stop = true;
            winner = symbols[i];
            EndGameStyle(0,0);
            EndGameStyle(1,1);
            EndGameStyle(2,2);
            return;
        }
        if((gameArea[0][2]==symbols[i]) and (gameArea[1][1]==symbols[i]) and (gameArea[2][0]==symbols[i]))
        {
            stop = true;
            winner = symbols[i];
            EndGameStyle(0,2);
            EndGameStyle(1,1);
            EndGameStyle(2,0);
            return;
        }
    }
    if(progress==9){
        stop = true;
    }
}
void Widget::endGame()
{
    playerLocked = true;
    if(stop)
    {
        if(winner == player)
        {
            ui->messageLabel->setText("Ты победил");
            ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
        }
        else if(winner=='-'){
            ui->messageLabel->setText("Ничья");
        }
        else{
            ui->messageLabel->setText("Ты проиграл");
            ui->messageLabel->setStyleSheet(StyleHelper::getLostMessageStyle());
        }
        ui->startButton->setText("Играть");
        if(interface == true)
            ui->startButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
        else
            ui->startButton->setStyleSheet(StyleHelper::getLightStartButtonStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;
    }
}

void Widget::EndGameStyle(int row,int colum)
{
    QString style;
    if(winner==player)
    {
        if(player=='X')
        {
            style = StyleHelper::getCrossVictoryStyle();
        } else{
            style = StyleHelper::getZeroVictoryStyle();
        }
    }else{
        if(player=='X'){
            style = StyleHelper::getZeroLostStyle();
        }else{
            style = StyleHelper::getCrossLostStyle();
        }
    }
    changeButtonStyle(row,colum,style);
}

void Widget::onComputerSlot()//Ход компьютера
{
    char comp;
    QString style;
    if(player=='X'){
        comp = '0';
        style = StyleHelper::getZeroNormalStyle();
    }
    else{
        comp = 'X';
        style = StyleHelper::getCrossNormalStyle();
    }
    timer->stop();

    while(true)
    {
        int row = rand()%3;
        int colum = rand()%3;
        if(gameArea[row][colum]=='-'){
            gameArea[row][colum] = comp;
            changeButtonStyle(row,colum,style);
            ui->messageLabel->setText("Твой ход!");
            progress++;
            chekGameStop();
            endGame();
            playerLocked = false;
            break;
        }
    }

}

void Widget::on_aboutButton_clicked()
{
    if(aboutGameclicked==false){
        ui->tabWidget->setCurrentIndex(1);
        aboutGameclicked = true;
    }else{
        ui->tabWidget->setCurrentIndex(0);
        aboutGameclicked = false;
    }
}


void Widget::on_darkButton_clicked()
{
    interface = true;
    this->setStyleSheet(StyleHelper::getDarkMainWidgetStyle());
    if(player=='X'){
        ui->leftButton->setStyleSheet(StyleHelper::getDarkLeftButtonActiveStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getDarkRightButtonStyle());
    }else{
        ui->leftButton->setStyleSheet(StyleHelper::getDarkLeftButtonStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getDarkRightButtonActiveStyle());
    }
    ui->label->setStyleSheet(StyleHelper::getDarkNameGameStyle());
    ui->aboutButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
    ui->startButton->setStyleSheet(StyleHelper::getDarkStartButtonStyle());
}


void Widget::on_lightButton_clicked()
{
    interface = false;
    this->setStyleSheet(StyleHelper::getLightMainWidgetStyle());
    if(player=='X'){
        ui->leftButton->setStyleSheet(StyleHelper::getLightLeftButtonActiveStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getLightRightButtonStyle());
    }else{
        ui->leftButton->setStyleSheet(StyleHelper::getLightLeftButtonStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getLightRightButtonActiveStyle());
    }
    ui->label->setStyleSheet(StyleHelper::getLightNameGameStyle());
    ui->aboutButton->setStyleSheet(StyleHelper::getLightStartButtonStyle());
    ui->startButton->setStyleSheet(StyleHelper::getLightStartButtonStyle());
}

