#include "goboardwindow.h"
#include "ui_goboardwindow.h"
#include <QWidget>
#include"boardstate.h"
#include<QPen>
#include <QPainter>
#include<QDebug>
#include"boardstate.h"
#include<QMouseEvent>
#include<QWidgetList>
#include<QPixmap>
#include<QTimer>
GoBoardWindow::GoBoardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoBoardWindow)
{
    ui->setupUi(this);
    this->music=new QSound(":/new/prefix1/luozi_music.wav");
    this->setMouseTracking(true);
    this->setFixedSize(980,980);
    QTimer *timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        this->rangzi=this->model.rangzi;
        this->tiemu=this->model.tiemu;
    });
    if(!this->model.isModelset)//如果模式还没有设置完成，就要一直刷新
        timer->start(100);
    QTimer *updateTimer=new QTimer(this);
    connect(updateTimer,&QTimer::timeout,this,[=](){
        this->update();
    });
    updateTimer->start(10);



}

GoBoardWindow::~GoBoardWindow()
{
    delete ui;
}

void GoBoardWindow::drawboard()
{
    QPixmap pix;
    pix.load(":/new/prefix1/Board1.jpg");
    QPainter painter(this);
    painter.drawPixmap(0, 0, pix);
    ////画棋盘格子////////
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(10,10,20*this->SPACE,20*this->SPACE);
    for(int i=1;i<20;i++)
    {
        painter.drawLine(10+i*this->SPACE,10+19*this->SPACE,10+i*this->SPACE,10+this->SPACE);
        painter.drawLine(10+19*this->SPACE,10+i*this->SPACE,10+this->SPACE,10+i*this->SPACE);
    }
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    //先画小星星
    for(auto i:{0,1,2})
        for(auto j:{0,1,2})
            painter.drawEllipse(QRect(10+4*this->SPACE+6*i*this->SPACE-0.6*this->STAR_SIZE,10+4*this->SPACE+6*j*this->SPACE-0.6*this->STAR_SIZE,0.6*2*this->STAR_SIZE,0.6*2*this->STAR_SIZE));
    //画大星星
    for(auto i:{0,2})
        for(auto j:{0,2})
            painter.drawEllipse(QRect(10+4*this->SPACE+6*i*this->SPACE-this->STAR_SIZE,10+4*this->SPACE+6*j*this->SPACE-this->STAR_SIZE,2*this->STAR_SIZE,2*this->STAR_SIZE));
    for(auto i:{1})
        for(auto j:{1})
            painter.drawEllipse(QRect(10+4*this->SPACE+6*i*this->SPACE-this->STAR_SIZE,10+4*this->SPACE+6*j*this->SPACE-this->STAR_SIZE,2*this->STAR_SIZE,2*this->STAR_SIZE));
}

void GoBoardWindow::drawPiece()
{
    QPainter painter(this);
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    for(int i=1;i<20;i++)
        for(int j=1;j<20;j++)
        {
            switch(this->board.graph[i][j])
            {
                case boardState::GO_BLACK:
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QRect(10+j*this->SPACE-this->PIECE_RADIUS,10+i*this->SPACE-this->PIECE_RADIUS,2*this->PIECE_RADIUS,2*this->PIECE_RADIUS));
                break;
                                            }
                case boardState::GO_WHITE:{
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QRect(10+j*this->SPACE-this->PIECE_RADIUS,10+i*this->SPACE-this->PIECE_RADIUS,2*this->PIECE_RADIUS,2*this->PIECE_RADIUS));
                break;
            }
            default:{break;}
            }
            if(this->curMode==GoBoardWindow::START)
            {
                switch(this->board.tosettle[i][j])
                {
                case boardState::GO_SELECTED_BLACK:{
                                    brush.setColor(Qt::black);
                                    painter.setBrush(brush);
                                    painter.drawEllipse(QRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE));
                                    this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;//清楚标记，防止标记存留
                                    break;
                }
                case boardState::GO_SELECTED_WHITE:{
                                brush.setColor(Qt::white);
                                painter.setBrush(brush);
                                painter.drawEllipse(QRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE));
                                this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;
                                break;
                }
                default:break;
                }
            }
        }
    if(this->board.totalStep!=0)
    {
        if(this->curMode==GoBoardWindow::START)
        {
            //画下最新下的棋子的小红点标记
            int i=this->board.recordStack[this->board.totalStep].x;
            int j=this->board.recordStack[this->board.totalStep].y;
            brush.setColor(Qt::red);
            painter.setBrush(brush);
            painter.drawEllipse(QRect(10+j*this->SPACE-this->CUR_SETTLE,10+i*this->SPACE-this->CUR_SETTLE,2*this->CUR_SETTLE,2*this->CUR_SETTLE));
            this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;
        }
    }
}

void GoBoardWindow::drawSign()
{
    QPainter painter(this);
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    for(int i=1;i<20;i++)
        for(int j=1;j<20;j++)
        {
            switch(this->board.graph[i][j])
            {
                case boardState::GO_BLACK:
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QRect(10+j*this->SPACE-this->PIECE_RADIUS,10+i*this->SPACE-this->PIECE_RADIUS,2*this->PIECE_RADIUS,2*this->PIECE_RADIUS));
                break;
                                            }
                case boardState::GO_WHITE:{
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QRect(10+j*this->SPACE-this->PIECE_RADIUS,10+i*this->SPACE-this->PIECE_RADIUS,2*this->PIECE_RADIUS,2*this->PIECE_RADIUS));
                break;
            }
            default:{break;}
            }
        }

    for(int i=1;i<=19;i++)
        for(int j=1;j<=19;j++)
        {
            switch (this->board.signGraph[i][j]) {
            case boardState::GO_SIGN_BLACK:{
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE);
                break;
            }
            case boardState::GO_SIGN_WHITE:{
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE);
                break;
            }
            default:break;
            }
            switch(this->board.tosettle[i][j])
            {
            case boardState::GO_SELECTED_BLACK:{
                                brush.setColor(Qt::gray);
                                painter.setBrush(brush);
                                painter.drawRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE);
                                this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;//清楚标记，防止标记存留
                                break;
            }
            case boardState::GO_SELECTED_WHITE:{
                            brush.setColor(Qt::gray);
                            painter.setBrush(brush);
                            painter.drawRect(10+j*this->SPACE-this->SIGNED_SIZE,10+i*this->SPACE-this->SIGNED_SIZE,2*this->SIGNED_SIZE,2*this->SIGNED_SIZE);
                            this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;
                            break;
            }
            default:break;
            }

        }
    if(this->board.totalStep!=0)
    {
        if(this->curMode==GoBoardWindow::START)
        {
            //画下最新下的棋子的小红点标记
            int i=this->board.recordStack[this->board.totalStep].x;
            int j=this->board.recordStack[this->board.totalStep].y;
            brush.setColor(Qt::red);
            painter.setBrush(brush);
            painter.drawEllipse(QRect(10+j*this->SPACE-this->CUR_SETTLE,10+i*this->SPACE-this->CUR_SETTLE,2*this->CUR_SETTLE,2*this->CUR_SETTLE));
            this->board.tosettle[i][j]=boardState::GO_SELECTED_NULL;
        }
    }
}

void GoBoardWindow::paintEvent(QPaintEvent *event)
{
    this->drawboard();
    if(this->curMode!=GoBoardWindow::BAN&&this->model.isModelset)
    {
        if(!this->isInitFinished)
        {
            this->rangzi=this->model.rangzi;
            this->tiemu=this->model.tiemu;
            this->board.initBoard(this->rangzi);
            this->isInitFinished=true;
        }
        this->drawPiece();
    }
    if(this->curMode==GoBoardWindow::TOSIGN)
    {
        if(!this->isInitFinished)
        {
            this->rangzi=this->model.rangzi;
            this->tiemu=this->model.tiemu;
            this->board.initBoard(this->rangzi);
            this->isInitFinished=true;
        }
        this->drawSign();
    }

}

void GoBoardWindow::mousePressEvent(QMouseEvent *event)
{

    if(this->curMode==GoBoardWindow::START)
    {
            this->music->play();
        int x,y,col=0,row=0;//鼠标的位置以及对应的行数列数
        x=static_cast<int>(event->x()-10);
        y=static_cast<int>(event->y()-10);//将远点移动到棋盘外边框
        col=x/this->SPACE;
        row=y/this->SPACE;
        col+=(x-col*this->SPACE)>this->SPACE/2?1:0;
        row+=(y-row*this->SPACE)>this->SPACE/2?1:0;
        /////改变相应点的状态/////
        //处理落子，改变board的各种状态
        if(this->board.isLegalToSettle(row,col,this->board.curTurn))//判断是否可以落子
        {
            this->board.GoSettle(row,col,this->board.curTurn);//落子
            this->board.boardUpdate();//更新棋盘
            update();
        }
    }
    //标记模式下进行标记
    else if(this->curMode==GoBoardWindow::TOSIGN)
    {
        int x,y,col=0,row=0;//鼠标的位置以及对应的行数列数
        x=static_cast<int>(event->x()-10);
        y=static_cast<int>(event->y()-10);//将远点移动到棋盘外边框
        col=x/this->SPACE;
        row=y/this->SPACE;
        col+=(x-col*this->SPACE)>this->SPACE/2?1:0;
        row+=(y-row*this->SPACE)>this->SPACE/2?1:0;
        //标记SIGN
        switch(event->button()){
        case Qt::LeftButton:{
            this->board.signGraph[row][col]=boardState::GO_SIGN_BLACK;//左键做黑色标记
                    this->board.MuUpdate();
            break;
        }
        case Qt::RightButton:{
            this->board.signGraph[row][col]=boardState::GO_SIGN_WHITE;//右键做黑色标记
                    this->board.MuUpdate();
            break;
        }
        case Qt::MidButton:{
            this->board.signGraph[row][col]=boardState::GO_SIGN_BLANK;//中键清楚标记
                    this->board.MuUpdate();
            break;
        }
        default:break;
            //更新标记
        }

    }
}
void GoBoardWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x,y,col=0,row=0;//鼠标的位置以及对应的行数列数
    x=static_cast<int>(event->x()-10);
    y=static_cast<int>(event->y()-10);//将远点移动到棋盘外边框
    /*以后可以修改这个间距让棋盘看起来更流畅*/
    col=x/this->SPACE;
    row=y/this->SPACE;
    col+=(x-col*this->SPACE)>this->SPACE/2?1:0;
    row+=(y-row*this->SPACE)>this->SPACE/2?1:0;
    if(row<0)
        row=0;
    else if(row>20)
        row=20;
    if(col<0)
        col=0;
    else if(col>20)
        col=20;
    this->curRow=row;
    this->curCol=col;
    /////改变相应点的状态/////
    //如果点击的不是边界点就要标记是哪个棋子
    if(!this->board.isPiece(row,col)&&!this->board.isSigned(row,col))
    {
        this->board.tosettle[row][col]=this->board.curTurn==boardState::GO_TURN_WHITE? boardState::GoSelect{boardState::GO_SELECTED_WHITE}:boardState::GoSelect{boardState::GO_SELECTED_BLACK};
        update();
    }
}
