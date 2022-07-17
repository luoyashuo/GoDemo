#include <QKeyEvent>
#include "mybutton.h"

MyButton::MyButton(QWidget *parent):
    QPushButton(parent)
{

}

void MyButton::keyPressEvent(QKeyEvent *e)
{
    int k = e->key();
    if (   k == Qt::Key_Up
           || k == Qt::Key_Down
           || k == Qt::Key_Left
           || k == Qt::Key_Right)
    {
        e->ignore();//让这些事件继续上传给父窗口，在本例中指SwitchButtonWidget
    }
    else
    {
        e->accept();//停止其它事件向上传播
    }
}
