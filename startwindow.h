#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include<QPaintEvent>
#include<QSound>
#include<QMediaPlayer>
QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
        QSound *music;
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:

    Ui::StartWindow *ui;
protected:
        void paintEvent(QPaintEvent* event);
};
#endif // STARTWINDOW_H
