#ifndef BOARDCONTAINORWINDOW_H
#define BOARDCONTAINORWINDOW_H

#include <QMainWindow>
#include"goboardwindow.h"
#include"modelchoose.h"
#include<QSound>
#include<QMediaPlayer>
#include<QMouseEvent>
namespace Ui {
class boardContainorWindow;
}

class boardContainorWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSound *music;
    GoBoardWindow *displayer;
    explicit boardContainorWindow(QWidget *parent = nullptr);
    ~boardContainorWindow();
    void dataUpdate();//槽函数用来更新数据
private:
    Ui::boardContainorWindow *ui;


protected:
    void paintEvent(QPaintEvent* event);


};

#endif // BOARDCONTAINORWINDOW_H
