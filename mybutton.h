#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget* parent = 0);

protected:
    void keyPressEvent(QKeyEvent* e);
};

#endif // MYBUTTON_H
