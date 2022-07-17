#ifndef GOBOARDWINDOW_H
#define GOBOARDWINDOW_H

#include <QWidget>
#include"boardstate.h"
#include<QMouseEvent>
#include<modelchoose.h>
#include<QSound>
#include<QMediaPlayer>
namespace Ui {
class GoBoardWindow;
}

class GoBoardWindow : public QWidget
{
    Q_OBJECT
public:
    //表示现在棋盘窗口所处的状态
    typedef enum _mode{
        BAN,//对局未开始
        START,//对局开始
//        JUDGE,//数子模式
//        DEDUCTION,//推演模式
        TOSIGN,//标记模式
    }Mode;
public:
        QSound *music;
    const int SPACE=48;//网格间距
    const int PIECE_RADIUS=23;//棋子半径
    const int STAR_SIZE=12;//用来存储星的大小
    const int CUR_SETTLE=6;//用来标记最近的落子的小红点
    const int SIGNED_SIZE=14;//被标记后显示的大小
    int posX,posY;
    boardState board;//用来存储棋盘的信息
    int rangzi=0;//存储设定的规则,让子数
    double tiemu=7.5;//贴目数
    int interruptStep;//推演模式下开始的步数
    Mode curMode=Mode{BAN};//初始模式设置为禁止
    int curRow=0,curCol=0;//鼠标当前所在的位置（行，列）
    ModelChoose model;//用于存储模式信息
    bool isInitFinished=false;
    explicit GoBoardWindow(QWidget *parent = nullptr);
    ~GoBoardWindow();
    void drawboard();//画棋盘的网格线
    void drawPiece();//画棋子,以及最新的落子点的标记,以及被标记的点
    void drawSign();//画出标记的棋子
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::GoBoardWindow *ui;
};

#endif // GOBOARDWINDOW_H
