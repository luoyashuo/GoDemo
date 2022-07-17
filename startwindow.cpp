#include "startwindow.h"
#include "ui_startwindow.h"
#include<QPixmap>
#include<QPen>
#include<QPainter>
#include<QFont>
#include<QDebug>
#include"boardcontainorwindow.h"
StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    this->music=new QSound(":/new/prefix1/WaterDrop.wav");
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("GoDemo\n围棋教学演示"));
    this->setWindowIcon(QIcon(":/new/prefix1/icon1.png"));
    this->setFixedSize(1920,1020);
    ui->btnesc->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btnstart->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btnhelp->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btnnew->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btnback->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btnintroduce->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btncloseBrowser->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->btncloseBrowser_2->setFixedSize((int)((int)(0.33*this->width())),(int)(0.1*this->height()));
    ui->textBrowser->setFixedSize((int)((int)(0.7*this->width())),(int)(0.35*this->height()));
    int d=-0.05*this->height();
    int w=-0.1*this->width();
    int x1=(int)((this->width()-ui->btnesc->width())*0.5);
    int y1=(int)((0.167*this->height()-ui->btnesc->height())*0.5+this->height()*0.5)+d;
    int x2=x1;
    int y2=(int)(y1+0.167*this->height())+d;
    int x3=x2;
    int y3=(int)(y2+0.167*this->height())+d;
    int x4=(int)((0.667*this->width()-ui->btnesc->width())/2)+w;
    int y4=(int)((0.25*this->height()-ui->btnesc->height())/2+this->height()*0.5)+d;
    int x5=(int)(this->width()-x2)+w;
    int y5=y4;
    int x6=x1;
    int y6=(int)(y4+0.25*this->height())+d;
    int x7=(int)((this->width()-ui->textBrowser->width())*0.5);
    int y7=y1+3*d;
    ui->btnstart->move(x1,y1);
    ui->btnhelp->move(x2,y2);
    ui->btnesc->move(x3,y3);
    ui->btnnew->move(x4,y4);
    ui->btnintroduce->move(x5,y5);
    ui->btnback->move(x6,y6);
    ui->btnintroduce->hide();
    ui->btnnew->hide();
    ui->btnback->hide();
    ui->btnstart->show();
    ui->btnesc->show();
    ui->btnhelp->show();
    ui->btncloseBrowser->move(x6,y6);
    ui->btncloseBrowser->hide();
    ui->btncloseBrowser_2->move(x6,y6);
    ui->btncloseBrowser_2->hide();
    ui->textBrowser->move(x7,y7);
    ui->textBrowser->hide();
    connect(ui->btnesc,&QPushButton::clicked,this,[=](){
        this->close();
            this->music->play();
    });
    connect(ui->btnstart,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->show();
        ui->btnnew->show();
        ui->btnback->show();
        ui->btnstart->hide();
        ui->btnesc->hide();
        ui->btnhelp->hide();
            this->music->play();
    });
    connect(ui->btnback,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->hide();
        ui->btnnew->hide();
        ui->btnback->hide();
        ui->btnstart->show();
        ui->btnesc->show();
        ui->btnhelp->show();
            this->music->play();
    });
    connect(ui->btnnew,&QPushButton::clicked,this,[=](){
        boardContainorWindow* p=new boardContainorWindow;
        p->show();
            this->music->play();
    });
    connect(ui->btnintroduce,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->hide();
        ui->btnnew->hide();
        ui->btnback->hide();
        ui->btnstart->hide();
        ui->btnesc->hide();
        ui->btnhelp->hide();
        QString str=QString((QStringLiteral("功能介绍：点击“新对局”开始教学，点击“开始”进入规则设置窗口，在规则设置窗口，可以选择自定义规则：在"
"填写贴目数以及选择让子数后开始对局，也可以用默认的中国规则开始对局；右下方显示当前对局的信息，中部注释区可以打字编辑，进行注释"
"在对局模式下：棋盘区点击相应的交叉点就可以落子。在对局界面右侧点击一次“悔棋”，可以悔棋一步；点击一次“停手”，交换下一步落子方；点击一次"
"“标记点目”，可以对交叉点进行标记，左键黑色，右键白色，中键取消标记，同时显示所有标记，再点击一次“退出标记”就可以继续对局，同时隐藏标记；点击一次"
"“推算模式”可以从当前步开始进行模拟推算，再点击一次“退出推算”可以返回正常对局状态，并且局面返回到推算前的状态；点击“结束”可以关闭当前窗口。")));
        ui->btncloseBrowser->show();
        ui->textBrowser->setText(str);
        ui->textBrowser->show();
                  this->music->play();
    });
    connect(ui->btncloseBrowser,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->show();
        ui->btnnew->show();
        ui->btnback->show();
        ui->btnstart->hide();
        ui->btnesc->hide();
        ui->btnhelp->hide();
        ui->btncloseBrowser->hide();
        ui->textBrowser->hide();
        ui->btncloseBrowser_2->hide();
                  this->music->play();
    });
    connect(ui->btnhelp,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->hide();
        ui->btnnew->hide();
        ui->btnback->hide();
        ui->btnstart->hide();
        ui->btnesc->hide();
        ui->btnhelp->hide();
        QString str=QString((QStringLiteral("作品说明:本作品主要目的用于在没有棋盘和棋子的条件下，通过网课或者投影仪的形势向学生讲授围棋，比如在"
"支教的时候可以通过线上会议或者线下的投影仪来向山村小学的同学讲授围棋，摆脱了器材的限制。相较于目前单机的围棋软件一般比较简陋，网络围棋软件主要面向在线对局的"
"情况，本作品定位是“单机+教学”。实现了围棋的所有规则：提子，反提，劫；以及为了便于教学的一些功能：标记最新落子处，标记点目功能，提子总数记录功能，中途推算"
"功能，悔棋功能，规则设置功能（让子，贴目）停手功能等，在右下角还可以显示对局的总步数，双方提子，双方标记目数，鼠标处的坐标，当前模式，当前规则等，"
"还增加了一个可以编辑的注释区，用来对当前局面进行解释（主要是面向网课设计的）提高了教学的性能。"
"\n作者：北京航空航天大学 软件学院 罗亚硕")));
        ui->btncloseBrowser->show();
        ui->textBrowser->setText(str);
        ui->textBrowser->show();
        ui->btncloseBrowser_2->show();
        this->music->play();
    });
    connect(ui->btncloseBrowser_2,&QPushButton::clicked,this,[=](){
        ui->btnintroduce->hide();
        ui->btnnew->hide();
        ui->btnback->hide();
        ui->btnstart->show();
        ui->btnesc->show();
        ui->btnhelp->show();
        ui->textBrowser->hide();
        ui->btncloseBrowser_2->hide();
        ui->btncloseBrowser->hide();
            this->music->play();
    });
}

StartWindow::~StartWindow()
{
    delete ui;
}
void StartWindow:: paintEvent(QPaintEvent* event){
    //画背景图片
    QPixmap pix;
    pix.load(":/new/prefix1/Background2.jpg");
    pix=pix.scaled(this->width(),this->height(),Qt::KeepAspectRatioByExpanding);
//    qDebug()<<"x="<<this->width()<<"\n"<<"y="<<this->height();
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
    //画艺术字
    QFont font;
    font.setPointSize(40);
    font.setBold(true);
    QFontMetrics metrics(font);
    QPainterPath path;
    QPen pen(QColor(110, 123, 139, 50));
    int penWidth = font.pointSize() * 0.5;
    if (penWidth > 6) {
        penWidth = 6;
    }
    pen.setWidth(penWidth);
    int len = metrics.width(QStringLiteral("GoDemo\n围棋教学演示"));
    int w = this->width();
    int px = (len - w) / 2;
    if (px < 0) {
        px = -px;
    }
    int py = (0.5*this->height() - metrics.height()) / 2 + metrics.ascent();
    if(py < 0)
    {
        py = -py;
    }
    path.addText(px, py, font, QStringLiteral("GoDemo\n围棋教学演示"));
    painter.strokePath(path, pen);
    painter.drawPath(path);
    painter.fillPath(path, QBrush(QColor(79,79,79,100)));
}


