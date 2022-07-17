#ifndef MODELCHOOSE_H
#define MODELCHOOSE_H

#include <QDialog>
#include<QSound>
#include<QMediaPlayer>
namespace Ui {
class ModelChoose;
}

class ModelChoose : public QDialog
{
    Q_OBJECT

public:
        QSound *music;
    double tiemu=7.5;
    int rangzi=0;
    bool isModelset=false;//用来储存当前模式是不是已经设置完毕
    explicit ModelChoose(QWidget *parent = nullptr);
    ~ModelChoose();

private:
    Ui::ModelChoose *ui;
protected:

};

#endif // MODELCHOOSE_H
