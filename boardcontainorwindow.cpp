#include "boardcontainorwindow.h"
#include "ui_boardcontainorwindow.h"
#include<QPixmap>
#include<QPen>
#include<QPainter>
#include<QFont>
#include<QDebug>
#include"modelchoose.h"
#include"goboardwindow.h"
#include<QDebug>
#include<QTimer>
#include<QString>
#include"boardstate.h"
boardContainorWindow::boardContainorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::boardContainorWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("GoDemo\n围棋教学演示"));
    this->setWindowIcon(QIcon(":/new/prefix1/icon1.png"));
    this->music=new QSound(":/new/prefix1/WaterDrop.wav");
    this->setFixedSize(1920,1020);
    this->displayer=new GoBoardWindow;
    this->displayer->setParent(this);
    this->displayer->move(0,0);
    /////点击开始游戏
    connect(ui->btnStart,&QPushButton::clicked,this,[=](){
        this->displayer->model.show();
        this->displayer->curMode=GoBoardWindow::START;
            this->music->play();
    });
    ////点击了结束
    connect(ui->btnClose,&QPushButton::clicked,this,[=](){
        this->close();
            this->music->play();
    });
    /////显示器
    QTimer *timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&boardContainorWindow::dataUpdate);
    timer->start(100);
    ///////悔棋按钮
    connect(ui->btnHuiqi,&QPushButton::clicked,this,[=](){
        this->displayer->board.boardRepentance();
               this->displayer->update();
            this->music->play();
    });
    /////停手按钮
    connect(ui->btnStop,&QPushButton::clicked,this,[=](){
       this->displayer->board.boardStopOneStep();
               this->displayer->update();
    });
    /////标记点目按钮,点一下进入TOSIGN模式，再点一下返回START模式
    connect(ui->btnToSign,&QPushButton::clicked,this,[=](){
       if(this->displayer->curMode==GoBoardWindow::START)
           this->displayer->curMode=GoBoardWindow::TOSIGN;
       this->displayer->update();
           this->music->play();
    });
    connect(ui->btnExitToSign,&QPushButton::clicked,this,[=](){
       if(this->displayer->curMode==GoBoardWindow::TOSIGN)
           this->displayer->curMode=GoBoardWindow::START;
       this->displayer->update();
           this->music->play();
    });
    ///推演模式按钮
    connect(ui->btnDeducation,&QPushButton::clicked,this,[=](){
        if(this->displayer->curMode==GoBoardWindow::START)
        {
            this->displayer->board.stopStep=this->displayer->board.totalStep;
                   this->displayer->update();
                this->music->play();
        }
    });
    connect(ui->btnStopDeducation,&QPushButton::clicked,this,[=](){
        if(this->displayer->curMode==GoBoardWindow::START)
        {
            this->displayer->board.stopDeducation(this->displayer->board.stopStep);
            this->displayer->board.stopStep=-1;//退出推演模式要把推演开始步数设置成-1
            this->displayer->update();
                this->music->play();
        }
    });

}

boardContainorWindow::~boardContainorWindow()
{
    delete ui;
}

void boardContainorWindow::dataUpdate()
{
    this->displayer->tiemu=this->displayer->model.tiemu;
    this->displayer->rangzi=this->displayer->model.rangzi;
    qDebug()<<"this->display->tiemu"<<this->displayer->tiemu<<" this->display->rangzi"<< this->displayer->rangzi<<"model->tiemu"<<this->displayer->model.tiemu<<"model->rangzi"<<displayer->model.rangzi;
    ui->lcdCurTotalStep->display(this->displayer->board.totalStep);
    ui->lcdBlackMu->display(this->displayer->board.muBlack);
    ui->lcdWhiteMu->display(this->displayer->board.muWhite);
    ui->lcdBlackTizi->display(this->displayer->board.tiziBlack);
    ui->lcdWhiteTizi->display(this->displayer->board.tiziWhite);
    if(this->displayer->curMode==GoBoardWindow::START&&this->displayer->board.stopStep==-1)
    {
      ui->labelCurModel->setText(QStringLiteral("对局模式"));
    }
    else if(this->displayer->curMode==GoBoardWindow::BAN)
    {
    ui->labelCurModel->setText(QStringLiteral("对局未开始"));
    }
    else if(this->displayer->curMode==GoBoardWindow::START&&this->displayer->board.stopStep!=-1)
    {
    ui->labelCurModel->setText(QStringLiteral("推演模式"));
    }
    else if(this->displayer->curMode==GoBoardWindow::TOSIGN)
    {
    ui->labelCurModel->setText(QStringLiteral("标记点目模式"));
    }
    else
    ui->labelCurModel->setText(QStringLiteral("NULL"));
    int row=this->displayer->curRow;
    row=20-row;//常识中以左下角的坐标原点
    int col=this->displayer->curCol;
    QString str2,str3,str4;
    str3=QString::number(row);
    str4=QString::number(col);
    str2="("+str3+","+str4+")";
    ui->labelTip->setText(str2);
    QString str6,str7,str8;
    str6= QString::number(this->displayer->tiemu,'f',1);
    str7=QString::number(this->displayer->rangzi);
    str8=QStringLiteral("贴")+str6+QStringLiteral("目，让")+str7+QStringLiteral("子");
    ui->labelGuize->setText(str8);
}
void boardContainorWindow:: paintEvent(QPaintEvent* event){
    //画背景图片
    QPixmap pix;
    pix.load(":/new/prefix1/BoardBackground1.jpg");
    pix=pix.scaled(this->width(),this->height(),Qt::KeepAspectRatioByExpanding);
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
}


